#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <cstddef>

#include "value.hpp"
#include "log.h"

namespace AST
{
namespace detail
{

class Context final
{
public:
    using Name  = AST::detail::Name;
    using Frame = AST::detail::Frame;

    std::ostream& out;
    FramePtr      current_;

    std::size_t callDepth_    = 0;
    std::size_t maxCallDepth_ = 1000;

public:
    explicit Context(std::ostream& outputStream = std::cout)
        : out(outputStream)
    {
        current_ = std::make_shared<Frame>();
        current_->parent = nullptr;

        LOG("CTX: init root frame ptr={}\n", static_cast<const void*>(current_.get()));
    }

    void push_scope()
    {
        auto newFrame = std::make_shared<Frame>();
             newFrame->parent = current_;

        LOG("CTX: push_scope: old={} new={} depth_before={}\n",
            static_cast<const void*>(current_.get()),
            static_cast<const void*>(newFrame.get()),
            depth());

        current_ = std::move(newFrame);

        LOG("CTX: push_scope: depth_after={}\n", depth());
    }

    void pop_scope()
    {
        if (!current_ || !current_->parent)
            throw std::runtime_error("pop_scope(): no parent scope (attempt to pop root scope)");

        LOG("CTX: pop_scope: cur={} parent={} depth_before={}\n",
            static_cast<const void*>(current_.get()),
            static_cast<const void*>(current_->parent.get()),
            depth());

        current_ = current_->parent;

        LOG("CTX: pop_scope: depth_after={}\n", depth());
    }

    void enter_scope() { push_scope(); }
    void exit_scope()  { pop_scope();  }

    FramePtr find_frame_with(Name name) const
    {
        for (auto framePointer = current_; framePointer; framePointer = framePointer->parent)
        {
            if (framePointer->vars.find(name) != framePointer->vars.end())
                return framePointer;
        }
        return nullptr;
    }

    Value assign(Name name, Value value)
    {
        if (!current_)
            throw std::runtime_error("assign(): current frame is null");

        if (auto foundFrame = find_frame_with(name))
        {
            foundFrame->vars[name] = std::move(value);
            LOG("CTX: assign update name='{}' in frame={}\n",
                name, static_cast<const void*>(foundFrame.get()));
            return foundFrame->vars.at(name);
        }

        current_->vars[name] = std::move(value);
        LOG("CTX: assign create name='{}' in frame={}\n",
            name, static_cast<const void*>(current_.get()));
        return current_->vars.at(name);
    }

    Value get_or_throw(Name name) const
    {
        Value outValue{};
        if (try_get(name, outValue))
            return outValue;

        throw std::runtime_error("Undeclared variable: " + std::string(name));
    }

    bool try_get(Name name, Value& out_value) const
    {
        if (auto foundFrame = find_frame_with(name))
        {
            out_value = foundFrame->vars.at(name);
            LOG("CTX: get name='{}' found in frame={}\n",
                name, static_cast<const void*>(foundFrame.get()));
            return true;
        }

        LOG("CTX: get name='{}' NOT FOUND\n", name);
        return false;
    }

    int depth() const
    {
        int depthValue = -1;
        for (auto framePointer = current_; framePointer; framePointer = framePointer->parent)
            ++depthValue;
        return depthValue;
    }

    void enter_call()
    {
        ++callDepth_;
        if (callDepth_ > maxCallDepth_)
            throw std::runtime_error("Maximum recursion depth exceeded ("   +
                                        std::to_string(callDepth_   ) + "/" +
                                        std::to_string(maxCallDepth_) + ")");
    }

    void exit_call()
    {
        if (callDepth_ > 0)
            --callDepth_;
    }

    std::size_t get_call_depth()     const { return callDepth_;    }
    std::size_t get_max_call_depth() const { return maxCallDepth_; }

    void set_max_call_depth(std::size_t newLimit) { maxCallDepth_ = newLimit; }
};

} // namespace detail

} // namespace AST
