#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "inode.hpp"

namespace AST
{

class NodeBuilder final
{
    std::vector<std::unique_ptr<INode>> nodes_;

public:
    NodeBuilder() = default;

    NodeBuilder(const NodeBuilder&)            = delete;
    NodeBuilder& operator=(const NodeBuilder&) = delete;

    template <class T, class... Args>
    T* create(Args&&... args)
    {
        static_assert(std::is_base_of_v<INode, T>, "T must derive from AST::INode");

        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T*   raw = ptr.get();
        nodes_.push_back(std::move(ptr));
        
        return raw;
    }

    void clear() { nodes_.clear(); }

    std::size_t size() const { return nodes_.size(); }
};

} // namespace AST
