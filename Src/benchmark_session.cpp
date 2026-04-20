#include "../Header/benchmark_session.hpp"

// Składanie domyślnej sesji: 3 adaptery ISorter, osobne pliki CSV na algorytm, wywołanie run_all.

#include <filesystem>
#include <fstream>
#include <locale>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../Header/isorter.hpp"
#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"

namespace {

std::string averages_output_path(const TestSettings& cfg) {
    const std::filesystem::path base_path(cfg.csv_path);
    const std::string filename =
        base_path.stem().string() + "-averages" + base_path.extension().string();
    const std::filesystem::path output_path =
        base_path.has_parent_path() ? base_path.parent_path() / filename
                                    : std::filesystem::path(filename);
    return output_path.string();
}

std::string sanitize_case_name_for_filename(const std::string& case_name) {
    std::string sanitized = case_name;
    for (char& character : sanitized) {
        if (character == '%') {
            character = 'p';
        } else if (character == '.') {
            character = '_';
        }
    }
    return sanitized;
}

std::map<std::string, std::string> case_averages_output_paths(const TestSettings& cfg) {
    std::map<std::string, std::string> paths;
    const std::filesystem::path base_path(cfg.csv_path);

    for (const CaseSpec& spec : all_cases()) {
        const std::string filename = base_path.stem().string() + "-case-"
                                     + sanitize_case_name_for_filename(spec.name)
                                     + base_path.extension().string();
        const std::filesystem::path output_path =
            base_path.has_parent_path() ? base_path.parent_path() / filename
                                        : std::filesystem::path(filename);
        paths[spec.name] = output_path.string();
    }

    return paths;
}

// Każdy adapter mapuje bool ascending z menu na sorting::SortOrder i woła szablon z nagłówka.
class MergeSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "merge_sort";
    }

    void sort(std::vector<int>& data, bool ascending) override {
        sorting::merge_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
    }
};

class QuickSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "quicksort";
    }

    void sort(std::vector<int>& data, bool ascending) override {
        sorting::quick_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
    }
};

class IntroSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "introsort";
    }

    void sort(std::vector<int>& data, bool ascending) override {
        sorting::intro_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
    }
};

// Adaptery ujednolicają 3 różne implementacje sortowania pod wspólne API benchmarku.
std::vector<std::unique_ptr<ISorter>> make_default_sorters() {
    std::vector<std::unique_ptr<ISorter>> sorters;
    sorters.push_back(std::make_unique<MergeSortAdapter>());
    sorters.push_back(std::make_unique<QuickSortAdapter>());
    sorters.push_back(std::make_unique<IntroSortAdapter>());
    return sorters;
}

RunArtifacts prepare_sorter_outputs(const TestSettings& cfg, const std::vector<ISorter*>& sorters) {
    RunArtifacts artifacts;
    const std::vector<std::string> output_paths = csv_output_paths(cfg, sorters);

    for (std::size_t index = 0; index < sorters.size(); ++index) {
        const std::filesystem::path output_path(output_paths[index]);
        if (output_path.has_parent_path()) {
            std::filesystem::create_directories(output_path.parent_path());
        }

        auto stream = std::make_unique<std::ofstream>(output_paths[index]);
        if (!stream->is_open()) {
            throw std::runtime_error("Could not open per-sorter CSV file.");
        }

        // Każdy plik dostaje ten sam nagłówek, żeby łatwo było je potem porównywać.
        stream->imbue(std::locale::classic());
        (*stream) << "row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly\n";

        artifacts.csv_streams[sorters[index]->name()] = stream.get();
        artifacts.owned_csv_streams[sorters[index]->name()] = std::move(stream);
    }

    const std::string averages_path = averages_output_path(cfg);
    const std::filesystem::path averages_path_fs(averages_path);
    if (averages_path_fs.has_parent_path()) {
        std::filesystem::create_directories(averages_path_fs.parent_path());
    }

    auto averages_stream = std::make_unique<std::ofstream>(averages_path);
    if (!averages_stream->is_open()) {
        throw std::runtime_error("Could not open averages CSV file.");
    }

    averages_stream->imbue(std::locale::classic());
    (*averages_stream) << "row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly\n";

    artifacts.csv_streams["all_averages"] = averages_stream.get();
    artifacts.owned_csv_streams["all_averages"] = std::move(averages_stream);

    const std::map<std::string, std::string> case_paths = case_averages_output_paths(cfg);
    for (const auto& [case_name, case_path] : case_paths) {
        const std::filesystem::path case_path_fs(case_path);
        if (case_path_fs.has_parent_path()) {
            std::filesystem::create_directories(case_path_fs.parent_path());
        }

        auto case_stream = std::make_unique<std::ofstream>(case_path);
        if (!case_stream->is_open()) {
            throw std::runtime_error("Could not open case averages CSV file.");
        }

        case_stream->imbue(std::locale::classic());
        (*case_stream) << "row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly\n";

        const std::string stream_key = "case_average:" + case_name;
        artifacts.csv_streams[stream_key] = case_stream.get();
        artifacts.owned_csv_streams[stream_key] = std::move(case_stream);
    }

    return artifacts;
}

}  // namespace

BenchmarkOutputs run_default_benchmark(const TestSettings& cfg,
                                       bool print_human,
                                       StatusCallback status_callback) {
    auto owned_sorters = make_default_sorters();
    std::vector<ISorter*> sorters;
    sorters.reserve(owned_sorters.size());

    for (const auto& sorter : owned_sorters) {
        sorters.push_back(sorter.get());
    }

    // Najpierw wyliczamy ścieżki, potem uruchamiamy jedną wspólną sesję benchmarku.
    BenchmarkOutputs outputs;
    outputs.csv_paths = csv_output_paths(cfg, sorters);
    outputs.averages_csv_path = averages_output_path(cfg);
    const std::map<std::string, std::string> case_paths = case_averages_output_paths(cfg);
    for (const auto& [_, path] : case_paths) {
        outputs.case_averages_csv_paths.push_back(path);
    }

    RunArtifacts artifacts = prepare_sorter_outputs(cfg, sorters);
    run_all(cfg, sorters, artifacts.csv_streams, print_human, std::move(status_callback));

    return outputs;
}
