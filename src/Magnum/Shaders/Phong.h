#ifndef Magnum_Shaders_Phong_h
#define Magnum_Shaders_Phong_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Class @ref Magnum::Shaders::Phong
 */

#include "Magnum/Color.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Shaders/Generic.h"
#include "Magnum/Shaders/visibility.h"

namespace Magnum { namespace Shaders {

/**
@brief %Phong shader

Uses ambient, diffuse and specular color or texture. For colored mesh you need
to provide @ref Position and @ref Normal attributes in your triangle mesh and
call at least @ref setTransformationMatrix(), @ref setNormalMatrix(),
@ref setProjectionMatrix(), @ref setDiffuseColor() and @ref setLightPosition().

If you want to use texture instead of color, you need to provide also
@ref TextureCoordinates attribute. Pass appropriate flags to constructor and
then at render time don't forget to also call appropriate subset of
@ref setAmbientTexture(), @ref setDiffuseTexture() and @ref setSpecularTexture().
*/
class MAGNUM_SHADERS_EXPORT Phong: public AbstractShaderProgram {
    public:
        typedef Generic3D::Position Position;   /**< @brief Vertex position */
        typedef Generic3D::Normal Normal;       /**< @brief Normal direction */

        /**
         * @brief %Texture coordinates
         *
         * Used only if one of @ref Flag::AmbientTexture, @ref Flag::DiffuseTexture
         * or @ref Flag::SpecularTexture is set.
         */
        typedef Generic3D::TextureCoordinates TextureCoordinates;

        #ifdef MAGNUM_BUILD_DEPRECATED
        enum: Int {
            /**
             * Ambient texture binding unit. Used only if
             * @ref Flag::AmbientTexture is set.
             * @deprecated Use @ref Magnum::Shaders::Phong::setAmbientTexture() "setAmbientTexture()"
             *      instead.
             */
            AmbientTextureLayer = 0,

            /**
             * Diffuse texture binding unit. Used only if
             * @ref Flag::DiffuseTexture is set.
             * @deprecated Use @ref Magnum::Shaders::Phong::setDiffuseTexture() "setDiffuseTexture()"
             *      instead.
             */
            DiffuseTextureLayer = 1,

            /**
             * Specular texture binding unit. Used only if
             * @ref Flag::SpecularTexture is set.
             * @deprecated Use @ref Magnum::Shaders::Phong::setSpecularTexture() "setSpecularTexture()"
             *      instead.
             */
            SpecularTextureLayer = 2
        };
        #endif

        /**
         * @brief %Flag
         *
         * @see @ref Flags, @ref flags()
         */
        enum class Flag: UnsignedByte {
            AmbientTexture = 1 << 0,    /**< The shader uses ambient texture instead of color */
            DiffuseTexture = 1 << 1,    /**< The shader uses diffuse texture instead of color */
            SpecularTexture = 1 << 2    /**< The shader uses specular texture instead of color */
        };

        /**
         * @brief %Flags
         *
         * @see @ref flags()
         */
        typedef Containers::EnumSet<Flag, UnsignedByte> Flags;

        /**
         * @brief Constructor
         * @param flags     %Flags
         */
        explicit Phong(Flags flags = Flags());

        /** @brief %Flags */
        Flags flags() const { return _flags; }

        /**
         * @brief Set ambient color
         * @return Reference to self (for method chaining)
         *
         * If not set, default value is `(0.0f, 0.0f, 0.0f)`. Has no effect if
         * @ref Flag::AmbientTexture is set.
         * @see @ref setAmbientTexture()
         */
        Phong& setAmbientColor(const Color3& color);

        /**
         * @brief Set ambient texture
         * @return Reference to self (for method chaining)
         *
         * Has effect only if @ref Flag::AmbientTexture is set.
         * @see @ref setTextures(), @ref setAmbientColor()
         */
        Phong& setAmbientTexture(Texture2D& texture);

        /**
         * @brief Set diffuse color
         * @return Reference to self (for method chaining)
         *
         * Has no effect if @ref Flag::DiffuseTexture is used.
         * @see @ref setDiffuseTexture()
         */
        Phong& setDiffuseColor(const Color3& color);

        /**
         * @brief Set diffuse texture
         * @return Reference to self (for method chaining)
         *
         * Has effect only if @ref Flag::DiffuseTexture is set.
         * @see @ref setTextures(), @ref setDiffuseColor()
         */
        Phong& setDiffuseTexture(Texture2D& texture);

        /**
         * @brief Set specular color
         * @return Reference to self (for method chaining)
         *
         * If not set, default value is `(1.0f, 1.0f, 1.0f)`. Has no effect if
         * @ref Flag::SpecularTexture is set.
         * @see @ref setSpecularTexture()
         */
        Phong& setSpecularColor(const Color3& color);

        /**
         * @brief Set specular texture
         * @return Reference to self (for method chaining)
         *
         * Has effect only if @ref Flag::SpecularTexture is set.
         * @see @ref setTextures(), @ref setSpecularColor()
         */
        Phong& setSpecularTexture(Texture2D& texture);

        /**
         * @brief Set textures
         * @return Reference to self (for method chaining)
         *
         * A particular texture has effect only if particular texture flag from
         * @ref Phong::Flag "Flag" is set, you can use `nullptr` for the rest.
         * More efficient than setting each texture separately.
         * @see @ref setAmbientTexture(), @ref setDiffuseTexture(),
         *      @ref setSpecularTexture()
         */
        Phong& setTextures(Texture2D* ambient, Texture2D* diffuse, Texture2D* specular);

        /**
         * @brief Set shininess
         * @return Reference to self (for method chaining)
         *
         * The larger value, the harder surface (smaller specular highlight).
         * If not set, default value is `80.0f`.
         */
        Phong& setShininess(Float shininess) {
            setUniform(shininessUniform, shininess);
            return *this;
        }

        /**
         * @brief Set transformation matrix
         * @return Reference to self (for method chaining)
         */
        Phong& setTransformationMatrix(const Matrix4& matrix) {
            setUniform(transformationMatrixUniform, matrix);
            return *this;
        }

        /**
         * @brief Set normal matrix
         * @return Reference to self (for method chaining)
         *
         * The matrix doesn't need to be normalized, as the renormalization
         * must be done in the shader anyway.
         */
        Phong& setNormalMatrix(const Matrix3x3& matrix) {
            setUniform(normalMatrixUniform, matrix);
            return *this;
        }

        /**
         * @brief Set projection matrix
         * @return Reference to self (for method chaining)
         */
        Phong& setProjectionMatrix(const Matrix4& matrix) {
            setUniform(projectionMatrixUniform, matrix);
            return *this;
        }

        /**
         * @brief Set light position
         * @return Reference to self (for method chaining)
         */
        Phong& setLightPosition(const Vector3& light) {
            setUniform(lightUniform, light);
            return *this;
        }

        /**
         * @brief Set light color
         * @return Reference to self (for method chaining)
         *
         * If not set, default value is `(1.0f, 1.0f, 1.0f)`.
         */
        Phong& setLightColor(const Color3& color) {
            setUniform(lightColorUniform, color);
            return *this;
        }

    private:
        Int transformationMatrixUniform,
            projectionMatrixUniform,
            normalMatrixUniform,
            lightUniform,
            diffuseColorUniform,
            ambientColorUniform,
            specularColorUniform,
            lightColorUniform,
            shininessUniform;

        Flags _flags;
};

CORRADE_ENUMSET_OPERATORS(Phong::Flags)

inline Phong& Phong::setAmbientColor(const Color3& color) {
    if(!(_flags & Flag::AmbientTexture)) setUniform(ambientColorUniform, color);
    return *this;
}

inline Phong& Phong::setDiffuseColor(const Color3& color) {
    if(!(_flags & Flag::DiffuseTexture)) setUniform(diffuseColorUniform, color);
    return *this;
}

inline Phong& Phong::setSpecularColor(const Color3& color) {
    if(!(_flags & Flag::SpecularTexture)) setUniform(specularColorUniform, color);
    return *this;
}

}}

#endif
