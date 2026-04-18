#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <locale>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../Header/ISorter.hpp"
#include "../Header/Tests.hpp"
#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"

namespace {

class MergeSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "merge_sort";
    }

    void sort(int* first, int* last, bool ascending) override {
        std::vector<int> data(first, last);
        sorting::merge_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
        std::copy(data.begin(), data.end(), first);
    }
};

class QuickSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "quicksort";
    }

    void sort(int* first, int* last, bool ascending) override {
        std::vector<int> data(first, last);
        sorting::quick_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
        std::copy(data.begin(), data.end(), first);
    }
};

class IntroSortAdapter : public ISorter {
public:
    const char* name() const override {
        return "introsort";
    }

    void sort(int* first, int* last, bool ascending) override {
        std::vector<int> data(first, last);
        sorting::intro_sort(
            data,
            ascending ? sorting::SortOrder::Ascending : sorting::SortOrder::Descending);
        std::copy(data.begin(), data.end(), first);
    }
};

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

        stream->imbue(std::locale::classic());
        (*stream) << "row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly\n";

        artifacts.csv_streams[sorters[index]->name()] = stream.get();
        artifacts.owned_csv_streams[sorters[index]->name()] = std::move(stream);
    }

    return artifacts;
}

}  // namespace

int main() {
    MergeSortAdapter merge_sort;
    QuickSortAdapter quicksort;
    IntroSortAdapter introsort;

    std::vector<ISorter*> sorters = {&merge_sort, &quicksort, &introsort};

    TestSettings settings;
    settings.repetitions_per_case = 10;
    settings.base_seed = 20260418u;
    settings.csv_path = "Results/results.csv";

    RunArtifacts artifacts = prepare_sorter_outputs(settings, sorters);
    run_all(settings, sorters, artifacts.csv_streams, true);

    return 0;
}
