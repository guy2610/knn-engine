#include <span>
#include <vector>

int main() {
    const std::vector<int> values{1, 2, 3};
    const std::span<const int> view{values};

    return view.size() == 3 ? 0 : 1;
}