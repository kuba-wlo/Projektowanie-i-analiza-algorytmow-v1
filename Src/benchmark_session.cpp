#include "../Header/benchmark_session.hpp"

// Składanie domyślnej sesji: 3 adaptery ISorter, osobne pliki CSV na algorytm, wywołanie run_all.

#include <filesystem>
#include <fstream>
#include <locale>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../Header/isorter.hpp"
#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"

namespace {

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

    RunArtifacts artifacts = prepare_sorter_outputs(cfg, sorters);
    run_all(cfg, sorters, artifacts.csv_streams, print_human, std::move(status_callback));

    return outputs;
}
