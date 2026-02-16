#include "ScarletEnginePch.h"
#include "Mesh.h"

#include "MeshLoader.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace Scarlet::Resource
{

Mesh::Mesh(const char* filepath)
{
    MeshLoader::LoadMesh(filepath, mMeshData);

    mVertexBuffer = new VertexBuffer(mMeshData.vertices.data(), mMeshData.vertices.size() * sizeof(Vertex));
    mIndexBuffer  = new IndexBuffer(mMeshData.indices.data(), mMeshData.indices.size());

    mVertexBuffer->PushVertexLayoutElement<float>(3);
    mVertexBuffer->PushVertexLayoutElement<float>(3);
    mVertexBuffer->PushVertexLayoutElement<float>(2);

    // Per Instance.
    mVertexBuffer->PushInstanceLayoutElement<float>(4);
    mVertexBuffer->PushInstanceLayoutElement<float>(4);
    mVertexBuffer->PushInstanceLayoutElement<float>(4);
    mVertexBuffer->PushInstanceLayoutElement<float>(4);

    mVertexArray = new VertexArray();
    mVertexArray->AddBuffer(*mVertexBuffer);
}

Mesh::~Mesh()
{
    delete mIndexBuffer;
    delete mIndexBuffer;
}

void Mesh::Bind() const
{
    mVertexArray->Bind();
    mVertexBuffer->Bind();
    mIndexBuffer->Bind();
}

} // Namespace Scarlet::Resource.