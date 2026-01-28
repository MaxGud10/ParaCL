#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <set>
#include <string_view>

#include "inode.hpp"

namespace AST
{

class NodeBuilder final
{
    std::vector<std::unique_ptr<INode>> nodes_;
    std::set   <std::string>            namePool_;

public:
    NodeBuilder()                              = default;
    NodeBuilder(const NodeBuilder&)            = delete;

    NodeBuilder& operator=(const NodeBuilder&) = delete;

    std::string_view intern(std::string_view s)
    {
        auto [it, inserted] = namePool_.emplace(s);

        return *it; 
    }

    template <class T, class... Args>
    T* create(Args&&... args)
    {
        static_assert(std::is_base_of_v<INode, T>, "T must derive from AST::INode");

        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T*   raw = ptr.get();
        nodes_.push_back(std::move(ptr));

        return raw;
    }

    void clear() 
    { 
        nodes_   .clear(); 
        namePool_.clear();
    }

    std::size_t size() const { return nodes_.size(); }
};

} // namespace AST
