#pragma once

namespace Scarlet
{

/**
 * @class Framebuffer: A class representing a framebuffer of the graphics pipeline. A framebuffer is a rendering destination. \\n
 * Currently the \ref Framebuffer automatically creates a colour attachment texture and depth stencil attachment (24-8).
 */
class Framebuffer
{
public:
    /**
     * @brief Construct a framebuffer with a given width and height.
     * @param width: The width of the framebuffer.
     * @param height: The height of the framebuffer.
     */
    Framebuffer(const uint32 width, const uint32 height);
    ~Framebuffer();

    Framebuffer(const Framebuffer&)            = delete;
    Framebuffer(Framebuffer&&)                 = delete;
    Framebuffer& operator=(Framebuffer&&)      = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    /**
     * @brief Bind the framebuffer so the graphics pipeline uses this framebuffer.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound framebuffer.
     */
    static void Unbind();

    void Resize(const uint32 width, const uint32 height);

    /**
     * @brief Get the identifier of the colour attachment of the framebuffer.
     * @return The identifier of the colour attachment of the framebuffer.
     */
    [[nodiscard]] inline uint32 GetColorAttachmentId() const { return mColorAttachmentId; }
private:
    uint32 mId;
    uint32 mColorAttachmentId;
    uint32 mDepthStencilAttachmentId;

    /**
     * @brief Create a framebuffer with its colour and depth-stencil attachment.
     * @param width The width of the framebuffer.
     * @param height The height of the framebuffer.
     */
    void Create(const uint32 width, const uint32 height);
    /**
     * @brief Free the framebuffer as well as its colour and depth-stencil attachment.
     */
    void Free();
};

} // Namespace Scarlet.