#ifndef coord_hh_INCLUDED
#define coord_hh_INCLUDED

#include "units.hh"
#include "hash.hh"

namespace Kakoune
{

template<typename EffectiveType, typename LineType, typename ColumnType>
struct LineAndColumn
{
    LineType   line = 0;
    ColumnType column = 0;

    [[gnu::always_inline]]
    constexpr EffectiveType operator+(EffectiveType other) const
    {
        return {line + other.line, column + other.column};
    }

    [[gnu::always_inline]]
    EffectiveType& operator+=(EffectiveType other)
    {
        line   += other.line;
        column += other.column;
        return *static_cast<EffectiveType*>(this);
    }

    [[gnu::always_inline]]
    constexpr EffectiveType operator-(EffectiveType other) const
    {
        return {line - other.line, column - other.column};
    }

    [[gnu::always_inline]]
    EffectiveType& operator-=(EffectiveType other)
    {
        line   -= other.line;
        column -= other.column;
        return *static_cast<EffectiveType*>(this);
    }

    constexpr friend auto operator<=>(const LineAndColumn& lhs, const LineAndColumn& rhs) = default;
    constexpr friend bool operator==(const LineAndColumn& lhs, const LineAndColumn& rhs) = default;

    friend constexpr size_t hash_value(const EffectiveType& val)
    {
        return hash_values(val.line, val.column);
    }
};

struct BufferCoord : LineAndColumn<BufferCoord, LineCount, ByteCount>
{
    [[gnu::always_inline]]
    constexpr BufferCoord(LineCount line = 0, ByteCount column = 0)
        : LineAndColumn{line, column} {}

    constexpr friend auto operator<=>(const BufferCoord& lhs, const BufferCoord& rhs) = default;
    constexpr friend bool operator==(const BufferCoord& lhs, const BufferCoord& rhs) = default;
};

struct DisplayCoord : LineAndColumn<DisplayCoord, LineCount, ColumnCount>
{
    [[gnu::always_inline]]
    constexpr DisplayCoord(LineCount line = 0, ColumnCount column = 0)
        : LineAndColumn{line, column} {}

    constexpr friend auto operator<=>(const DisplayCoord& lhs, const DisplayCoord& rhs) = default;
    constexpr friend bool operator==(const DisplayCoord& lhs, const DisplayCoord& rhs) = default;

    static constexpr const char* option_type_name = "coord";
};

struct BufferCoordAndTarget : BufferCoord
{
    [[gnu::always_inline]]
    constexpr BufferCoordAndTarget(LineCount line = 0, ByteCount column = 0, ColumnCount target = -1)
        : BufferCoord{line, column}, target{target} {}

    [[gnu::always_inline]]
    constexpr BufferCoordAndTarget(BufferCoord coord, ColumnCount target = -1)
        : BufferCoord{coord}, target{target} {}

    ColumnCount target;
};

constexpr size_t hash_value(const BufferCoordAndTarget& val)
{
    return hash_values(val.line, val.column, val.target);
}

}

#endif // coord_hh_INCLUDED
