/*
 * enumerate.hpp
 *
 * MIT License
 *
 * Copyright (c) 2017 Nico Madysa
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ENUMERATE_HPP
#define ENUMERATE_HPP

#include <type_traits>


namespace enumerate {

/// Base class that wraps the functionality of normal and reverse iterator.
template<typename Enum>
class EnumIterBase {
public:
    /// `Enum`.
    using value_type = Enum;

    /// The integer type underlying `Enum`.
    using integral_type = typename std::underlying_type<value_type>::type;

    /// Wrap an `enum` item in an iterator.
    constexpr explicit EnumIterBase(Enum inner) noexcept
        : m_inner(inner)
    {}

    constexpr EnumIterBase(const EnumIterBase&) = default;
    EnumIterBase& operator =(const EnumIterBase&) = default;

    /// Return the wrapped `enum` item.
    constexpr value_type operator *() const { return m_inner; }

    /// Iterators are equal if their wrapped `enum` items are equal.
    constexpr bool operator ==(EnumIterBase rhs) const {
        return m_inner == rhs.m_inner;
    }

    /// Iterators differ if their wrapped `enum` items differ.
    constexpr bool operator !=(EnumIterBase rhs) const {
        return m_inner != rhs.m_inner;
    }

protected:
    /// Convert the wrapped item to its corresponding integer value.
    constexpr integral_type unwrap() const {
        return static_cast<integral_type>(m_inner);
    }

    /// Wrap the `enum` item corresponding to the integer `i`.
    void wrap(integral_type i) {
        m_inner = static_cast<value_type>(i);
    }

    /// The wrapped `enum` item.
    value_type m_inner;
};


/// A simple forwards iterator over any `enum`.
template<typename Enum>
class EnumIter : public EnumIterBase<Enum> {
    /// Alias of the base class for dependent-name lookup.
    using base_type = EnumIterBase<Enum>;

public:
    using value_type = typename base_type::value_type;
    using integral_type = typename base_type::integral_type;

    /// Inherited base class constructor.
    using base_type::base_type;

    /// Increment the wrapped `enum` item.
    EnumIter operator ++() {
        base_type::wrap(base_type::unwrap()+1);
        return *this;
    }

    /// Decrement the wrapped `enum` item.
    EnumIter operator --() {
        base_type::wrap(base_type::unwrap()-1);
        return *this;
    }
};


/**A simple backwards iterator over any `enum`.
 *
 * Beware that `operator ++` and `operator --` do the opposite of their
 * conventional meaning; they decrement and increment the wrapped value
 * respectively.
 */
template<typename Enum>
class ReverseEnumIter : public EnumIterBase<Enum> {
    /// Alias of the base class for dependent-name lookup.
    using base_type = EnumIterBase<Enum>;

public:
    using value_type = typename base_type::value_type;
    using integral_type = typename base_type::integral_type;

    /// Inherited base class constructor.
    using base_type::base_type;

    /// Decrement the wrapped `enum` item.
    ReverseEnumIter operator ++() {
        base_type::wrap(base_type::unwrap()-1);
        return *this;
    }

    /// Increment the wrapped `enum` item.
    ReverseEnumIter operator --() {
        base_type::wrap(base_type::unwrap()+1);
        return *this;
    }
};


/**Iterate over all items in an `enum`.
 *
 * This allows using an `enum` in a `range-for` loop:
 *
 * ```
 * using enumerate::Enumerate;
 * for (const auto fruit : Enumerate<Fruit>()) {
 *     std::cout << name(fruit) << std::endl;
 * }
 * ```
 *
 * As can be seen, an `enum` must adhere to a certain *protocol* in
 * order to be passed to `enumerate`:
 * - The `enum` must define a special item `BEGIN` that is equal
 *   to the first item in the range.
 * - The `enum` must define a special item `END` that is by `1` greater
 *   than the last item in the range.
 * - `BEGIN` must be smaller than or equal to `END`.
 * - Semantically, each value between `BEGIN` and `END` should be a
 *   valid `enum` item. This is not enforced, however.
 *
 * Consequently, `Enumerate` works best if the passed `enum` does not
 * explicitly specify the value of any enum item except of `BEGIN`.
 *
 * \see `enumerate`, the variable template serving the same purpose.
 */
template<typename Enum>
struct Enumerate {
    /// `EnumRange`s are compile-time constants.
    constexpr Enumerate() = default;

    /// `Enum`.
    using value_type = Enum;

    /// The corresponding forwards iterator.
    using iterator = EnumIter<value_type>;

    /// The corresponding backwards iterator.
    using reverse_iterator = ReverseEnumIter<value_type>;

    /// The integer type underlying `Enum`.
    using integral_type = typename iterator::integral_type;

    // The following lines specify the `enumerate` protocol.

    /// The initial value required by the `enumerate` protocol.
    static constexpr value_type begin_value = value_type::BEGIN;

    /// The past-the-end value required by the `enumerate` protocol.
    static constexpr value_type end_value = value_type::END;

    // The initial value must be smaller than the final value.
    static_assert(begin_value <= end_value);

    /// Return an iterator to the `enum`'s initial value.
    constexpr iterator begin() const {
        return iterator{begin_value};
    }

    /// Return an iterator to the `enum`'s past-the-end value.
    constexpr iterator end() const {
        return iterator{end_value};
    }

    /// Return a reverse iterator to the `enum`'s last value.
    constexpr reverse_iterator rbegin() const {
        // The ++ pushes the iterator from past-the-end to the end.
        return ++reverse_iterator{end_value};
    }

    /// Return a reverse iterator to the `enum`'s past-the-begin value.
    constexpr reverse_iterator rend() const {
        // The ++ pushes the iterator from the beginnin to before the
        // beginning.
        return ++reverse_iterator{begin_value};
    }
};


#ifdef __cpp_variable_templates
/**Variable template that is equivalent to `Enumerate`.
 *
 * The only difference to `Enumerate` is that this variable does not
 * need parentheses in a `range-for` loop.
 *
 * Usage:
 *
 * ```
 * using enumerate::enumerate;
 *
 * for (const auto fruit : enumerate<Fruit>) {
 *     std::cout << name(fruit) << "\n";
 * }
 * ```
 */
template<typename Enum>
static constexpr auto enumerate = Enumerate<Enum>{};
#endif

}

#endif // ENUMERATE_HPP
