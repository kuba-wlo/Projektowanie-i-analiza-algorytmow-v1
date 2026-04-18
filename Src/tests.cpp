#include "../Header/tests.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <numeric>
#include <random>
#include <stdexcept>

namespace {

std::ostream* resolve_csv_stream(const std::map<std::string, std::ostream*>& csv_out,
                                 const char* sorter_name) {
    // Pozwala zapisać albo do pliku konkretnego sortera, albo do wspólnego strumienia "all".
    if (const auto iterator = csv_out.find(sorter_name); iterator != csv_out.end()) {
        return iterator->second;
    }
    if (const auto iterator = csv_out.find("all"); iterator != csv_out.end()) {
        return iterator->second;
    }
    return nullptr;
}

}  // namespace

const std::vector<CaseSpec>& all_cases() {
    static const std::vector<CaseSpec> cases = {
        {CaseKind::RandomAll, "random"},
        {CaseKind::Prefix25, "prefix_25%"},
        {CaseKind::Prefix50, "prefix_50%"},
        {CaseKind::Prefix75, "prefix_75%"},
        {CaseKind::Prefix95, "prefix_95%"},
        {CaseKind::Prefix99, "prefix_99%"},
        {CaseKind::Prefix99_7, "prefix_99.7%"},
        {CaseKind::Reversed, "reversed"}
    };

    return cases;
}

void make_case(int* first, int* last, CaseKind kind, unsigned int seed) {
    const std::size_t size = static_cast<std::size_t>(last - first);
    std::mt19937 generator(seed);

    // Każdy przypadek startuje z tej samej uporządkowanej tablicy 0..n-1.
    for (std::size_t index = 0; index < size; ++index) {
        first[index] = static_cast<int>(index);
    }

    double shuffle_ratio = 0.0;
    switch (kind) {
        case CaseKind::RandomAll:
            shuffle_ratio = 1.0;
            break;
        case CaseKind::Prefix25:
            shuffle_ratio = 0.75;
            break;
        case CaseKind::Prefix50:
            shuffle_ratio = 0.50;
            break;
        case CaseKind::Prefix75:
            shuffle_ratio = 0.25;
            break;
        case CaseKind::Prefix95:
            shuffle_ratio = 0.05;
            break;
        case CaseKind::Prefix99:
            shuffle_ratio = 0.01;
            break;
        case CaseKind::Prefix99_7:
            shuffle_ratio = 0.003;
            break;
        case CaseKind::Reversed:
            std::reverse(first, last);
            return;
    }

    if (shuffle_ratio > 0.0) {
        const std::size_t shuffle_count = static_cast<std::size_t>(size * shuffle_ratio);
        // Tasujemy tylko końcówkę, dzięki czemu początek pozostaje w zadanym stopniu posortowany.
        std::shuffle(last - static_cast<std::ptrdiff_t>(shuffle_count), last, generator);
    }
}

bool is_sorted(const int* first, const int* last, bool ascending) {
    if (first >= last) {
        return true;
    }

    for (const int* current = first; current < last - 1; ++current) {
        if (ascending ? (*(current + 1) < *current) : (*(current + 1) > *current)) {
            return false;
        }
    }

    return true;
}

double measure_sort_ms(ISorter& sorter, std::vector<int>& data, bool ascending) {
    // Mierzymy samo sortowanie na buforze roboczym, bez kopiowania do adaptera i z powrotem.
    const auto start_time = std::chrono::high_resolution_clock::now();
    sorter.sort(data, ascending);
    const auto end_time = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

RunArtifacts prepare_outputs(const TestSettings& cfg, bool print_csv) {
    RunArtifacts artifacts;

    if (!print_csv) {
        return artifacts;
    }

    const std::filesystem::path output_path(cfg.csv_path);
    if (output_path.has_parent_path()) {
        std::filesystem::create_directories(output_path.parent_path());
    }

    auto stream = std::make_unique<std::ofstream>(cfg.csv_path);
    if (!stream->is_open()) {
        throw std::runtime_error("Could not open CSV output file.");
    }

    stream->imbue(std::locale::classic());
    (*stream) << "row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly\n";

    artifacts.csv_streams["all"] = stream.get();
    artifacts.owned_csv_streams["all"] = std::move(stream);

    return artifacts;
}

std::vector<std::string> csv_output_paths(const TestSettings& cfg,
                                          const std::vector<ISorter*>& sorters) {
    std::vector<std::string> paths;
    const std::filesystem::path base_path(cfg.csv_path);

    for (ISorter* sorter : sorters) {
        // Przekształcamy np. "results.csv" w "results-merge_sort.csv".
        const std::string filename =
            base_path.stem().string() + "-" + sorter->name() + base_path.extension().string();
        const std::filesystem::path output_path =
            base_path.has_parent_path() ? base_path.parent_path() / filename
                                        : std::filesystem::path(filename);
        paths.push_back(output_path.string());
    }

    return paths;
}

void run_all(const TestSettings& cfg,
             const std::vector<ISorter*>& sorters,
             const std::map<std::string, std::ostream*>& csv_out,
             bool print_human,
             StatusCallback status_callback) {
    if (print_human) {
        std::cout << "\nStarting benchmark session\n";
    }
    if (status_callback) {
        status_callback("Starting benchmark session.");
    }

    std::cout << std::fixed << std::setprecision(4);

    for (const std::size_t size : cfg.sizes) {
        if (print_human) {
            std::cout << "  [" << size << "] ";
            std::cout.flush();
        }
        if (status_callback) {
            status_callback("Running benchmarks for size " + std::to_string(size) + '.');
        }

        std::vector<int> base(size);
        std::vector<int> work(size);

        for (const CaseSpec& spec : all_cases()) {
            if (print_human) {
                std::cout << '.';
                std::cout.flush();
            }
            if (status_callback) {
                status_callback("  Case: " + std::string(spec.name));
            }

            std::map<std::string, double> accumulated_times;
            std::map<std::string, bool> all_valid;

            for (int repetition = 0; repetition < cfg.repetitions_per_case; ++repetition) {
                make_case(base.data(), base.data() + static_cast<std::ptrdiff_t>(size),
                          spec.kind, cfg.base_seed + static_cast<unsigned int>(repetition));

                for (ISorter* sorter : sorters) {
                    // Każdy sorter dostaje identyczne dane wejściowe w danej próbie.
                    work = base;
                    const char* sorter_name = sorter->name();

                    const double elapsed_ms = measure_sort_ms(*sorter, work, cfg.ascending);
                    const bool valid =
                        is_sorted(work.data(),
                                  work.data() + static_cast<std::ptrdiff_t>(work.size()),
                                  cfg.ascending);

                    accumulated_times[sorter_name] += elapsed_ms;
                    all_valid[sorter_name] =
                        all_valid.count(sorter_name) == 0 ? valid
                                                          : (all_valid[sorter_name] && valid);

                    std::ostream* stream = resolve_csv_stream(csv_out, sorter_name);

                    if (stream != nullptr) {
                        (*stream) << std::fixed << std::setprecision(6);
                        (*stream) << "trial;" << sorter_name << ';' << size << ';'
                                  << spec.name << ';' << repetition << ';' << elapsed_ms << ';'
                                  << (valid ? 1 : 0) << '\n';
                    }
                }
            }

            for (ISorter* sorter : sorters) {
                const char* sorter_name = sorter->name();
                std::ostream* stream = resolve_csv_stream(csv_out, sorter_name);

                if (stream != nullptr) {
                    // Wiersz average podsumowuje wszystkie próby dla danego rozmiaru i przypadku.
                    const double average_time = accumulated_times[sorter_name] / cfg.repetitions_per_case;

                    (*stream) << std::fixed << std::setprecision(6);
                    (*stream) << "average;" << sorter_name << ';' << size << ';'
                              << spec.name << ';' << -1 << ';' << average_time << ';'
                              << (all_valid[sorter_name] ? 1 : 0) << '\n';
                }
            }
        }

        if (print_human) {
            std::cout << " OK\n";
        }
        if (status_callback) {
            status_callback("Finished size " + std::to_string(size) + '.');
        }
    }

    if (status_callback) {
        status_callback("Benchmark session completed.");
    }
}
