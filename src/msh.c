#include "msh.h"
#include "glad/gl.h"

void msh_create(msh_t* msh, msh_desc_t* desc) {
  *msh = (msh_t){0};

  GladGLContext* gl = desc->gl;
  msh->gl = desc->gl;

  gl->GenVertexArrays(1, &msh->buf.vao);
  gl->BindVertexArray(msh->buf.vao);

  gl->GenBuffers(1, &msh->buf.vbo);
  gl->BindBuffer(GL_ARRAY_BUFFER, msh->buf.vbo);
  gl->BufferData(GL_ARRAY_BUFFER, desc->stride * desc->vertices_count,
                 desc->vertices, GL_STATIC_DRAW);

  for (size_t i = 0; i < desc->msh_attrib_count; i++) {
    msh_attrib_t next = desc->attribs[i];
    gl->VertexAttribPointer(next.location, next.component_count, next.type,
                            next.normalized, desc->stride,
                            (const void*)next.offset);

    gl->EnableVertexAttribArray(desc->attribs[i].location);
  }

  msh->buf.count = desc->vertices_count;

  msh->initialized = 1;
}

void msh_draw(msh_t* msh) {
  if (!msh->initialized)
    return;

  GladGLContext* gl = msh->gl;

  gl->BindVertexArray(msh->buf.vao);
  gl->DrawArrays(GL_TRIANGLES, 0, msh->buf.count);
}

void msh_destroy(msh_t* msh) {
  GladGLContext* gl = msh->gl;
  if (!msh->initialized)
    return;

  if (msh->buf.vao != 0) {
    gl->DeleteVertexArrays(1, &msh->buf.vao);
    msh->buf.vao = 0;
  }

  if (msh->buf.vbo != 0) {
    gl->DeleteBuffers(1, &msh->buf.vbo);
    msh->buf.vbo = 0;
  }

  *msh = (msh_t){0};
}
