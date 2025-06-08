#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(const std::string &lhs, const std::string &rhs) const noexcept { return lhs.size() < rhs.size(); }
    bool operator()(const std::string &lhs, const std::string_view &rhs) const noexcept {
        return lhs.size() < rhs.size();
    }
    bool operator()(const std::string_view &lhs, const std::string &rhs) const noexcept {
        return lhs.size() < rhs.size();
    }
    bool operator()(const std::string_view &lhs, const std::string_view &rhs) const noexcept {
        return lhs.size() < rhs.size();
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(const std::string &lhs, const std::string &rhs) const noexcept { return lhs == rhs; }
    bool operator()(const std::string &lhs, const std::string_view &rhs) const noexcept { return lhs == rhs; }
    bool operator()(const std::string_view &lhs, const std::string &rhs) const noexcept { return lhs == rhs; }
    bool operator()(const std::string_view &lhs, const std::string_view &rhs) const noexcept { return lhs == rhs; }
};

struct TransparentStringHash {
    using is_transparent = void;

    std::size_t operator()(const std::string &str) const noexcept { return std::hash<std::string>{}(str); }

    std::size_t operator()(const std::string_view &str) const noexcept { return std::hash<std::string_view>{}(str); }
};

}  // namespace bookdb
