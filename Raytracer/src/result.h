#pragma once
#include <utility>
#include <new>
#include <type_traits>

template<typename T, typename E>
class Result {
public:
    bool is_error;
    union {
        T unwrap;
        E unwrap_err;
    };

    template <typename = std::enable_if_t<std::is_move_constructible<T>::value && std::is_move_constructible<E>::value>>
    Result(Result&& other) : is_error(other.is_error) {
        if(other.is_error) unwrap_err=std::move(other.unwrap_err);
        else unwrap=std::move(other.unwrap);
    }


    template <typename = std::enable_if_t<std::is_copy_constructible<T>::value && std::is_copy_constructible<E>::value>>
    Result(const Result& other) : is_error(other.is_error) {
        if(other.is_error) unwrap_err=other.unwrap_err;
        else unwrap=other.unwrap;
    }

    inline Result(const T& ok_value) : is_error(false) {
        new (&unwrap) T(ok_value);
    }

    inline Result(T&& ok_value) : is_error(false) {
        new (&unwrap) T(std::move(ok_value));
    }

    inline Result(const E& err_value) : is_error(true) {
        new (&unwrap_err) E(err_value);
    }

    inline Result(E&& err_value) : is_error(true) {
        new (&unwrap_err) E(std::move(err_value));
    }
    inline ~Result() {
        if(is_error) unwrap_err.~E();
        else unwrap.~T();
    }
};
