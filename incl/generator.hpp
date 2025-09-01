#pragma once
#include <coroutine>
#include <exception>
#include <utility>
#include <optional>
#include <variant>

template<typename T = void>
class Generator {
public:
    struct promise_type {
        T current_value{};
        std::exception_ptr exception;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept {
            current_value = std::move(value);
            return {};
        }
        void return_void() {}
        void unhandled_exception() { exception = std::current_exception(); }
    };
    
    using handle_type = std::coroutine_handle<promise_type>;
    
    // Default constructor - creates an empty/invalid generator
    Generator() : coro(nullptr) {}
    
    explicit Generator(handle_type h) : coro(h) {}
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    
    // Move constructor
    Generator(Generator&& other) noexcept : coro(other.coro) { 
        other.coro = nullptr; 
    }
    
    // Move assignment operator
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (coro) coro.destroy();
            coro = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }
    
    ~Generator() { if (coro) coro.destroy(); }
    
    bool next() {
        if (!coro || coro.done()) return false;
        coro.resume();
        if (coro.promise().exception) std::rethrow_exception(coro.promise().exception);
        return !coro.done();
    }
    
    T value() const { return coro.promise().current_value; }
    
    // Check if generator is valid
    bool valid() const { return coro != nullptr; }
    
private:
    handle_type coro;
};

// Specialization for void values
template<>
class Generator<void> {
public:
    struct promise_type {
        std::exception_ptr exception;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(std::monostate) noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { exception = std::current_exception(); }
    };
    
    using handle_type = std::coroutine_handle<promise_type>;
    
    // Default constructor - creates an empty/invalid generator
    Generator() : coro(nullptr) {}
    
    explicit Generator(handle_type h) : coro(h) {}
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    
    // Move constructor
    Generator(Generator&& other) noexcept : coro(other.coro) { 
        other.coro = nullptr; 
    }
    
    // Move assignment operator
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (coro) coro.destroy();
            coro = other.coro;
            other.coro = nullptr;
        }
        return *this;
    }
    
    ~Generator() { if (coro) coro.destroy(); }
    
    bool next() {
        if (!coro || coro.done()) return false;
        coro.resume();
        if (coro.promise().exception) std::rethrow_exception(coro.promise().exception);
        return !coro.done();
    }
    
    // Check if generator is valid
    bool valid() const { return coro != nullptr; }
    
private:
    handle_type coro;
};
