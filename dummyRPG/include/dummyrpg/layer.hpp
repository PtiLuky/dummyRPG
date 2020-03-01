#ifndef DUMMYRPG_LAYER
#define DUMMYRPG_LAYER

#include <vector>

#include "dummyrpg/dummy_types.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace Dummy {

const uint16_t MAX_LAYER_BORDER_SIZE = 1024;

///////////////////////////////////////////////////////////////////////////////

// Template class, must be fully defined in .hpp

///
/// \brief A Layer is a simple wrapper around a vector, with a <x,y> coordinates system
/// With some constrains as 0 <= x <= MAX_LAYER_BORDER_SIZE (same for y)
///
template <typename T> class Layer
{
public:
    Layer() {}

    ///
    /// \brief Layer Constructor of layer, fill width default value
    /// \param width wanted width, must not exceed MAX_LAYER_BORDER_SIZE
    /// \param height wanted height, must not exceed MAX_LAYER_BORDER_SIZE
    /// \param defaultValue default value to fill all cells
    ///
    Layer(uint16_t width, uint16_t height, T defaultValue)
    {
        if (width == 0)
            height = 0;
        else if (height == 0)
            width = 0;

        if (width > MAX_LAYER_BORDER_SIZE)
            width = MAX_LAYER_BORDER_SIZE;
        if (height > MAX_LAYER_BORDER_SIZE)
            height = MAX_LAYER_BORDER_SIZE;

        m_width   = width;
        m_height  = height;
        m_content = std::vector(width * height, defaultValue);
    }

    uint16_t width() const { return m_width; }
    uint16_t height() const { return m_height; }
    size_t size() const { return m_content.size(); }

    ///
    /// \brief at Get value at coordinates
    /// \param coords Coordinates where to read value. Must be valid.
    /// \return value read
    ///
    T at(tilecoords coords) const
    {
        uint32_t rowIdx = coords.second; // use uint32_t to avoid uint16 overflow
        return m_content[coords.first + rowIdx * m_width];
    }
    ///
    /// \brief set Set value at coordinates
    /// \param coords Coordinates where to write value. Must be valid.
    /// \param val Value to write.
    ///
    void set(tilecoords coords, T val)
    {
        uint32_t rowIdx                            = coords.second;
        m_content[coords.first + rowIdx * m_width] = val;
    }
    ///
    /// \brief areCoordValid check if coordinates are valid
    /// \param coords coords to check
    /// \return true if valid
    ///
    bool areCoordValid(tilecoords coords)
    {
        return (coords.first < m_width && coords.second < m_height);
    }

private:
    std::vector<T> m_content;
    uint16_t m_width  = 0;
    uint16_t m_height = 0;
};

///////////////////////////////////////////////////////////////////////////////

using GraphicLayer  = Layer<tileaspect>;
using BlockingLayer = Layer<bool>;

///////////////////////////////////////////////////////////////////////////////

} // namespace Dummy

#endif // DUMMYRPG_LAYER
