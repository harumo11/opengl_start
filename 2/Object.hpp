#pragma once
#include <GL/glew.h>
#include <array>

class Object {
   public:
    struct Vertex {
	GLfloat positions[2];
    };

    Object(GLuint size, GLsizei vertexcount, const Vertex *vertex);
    virtual ~Object();
    void bind() const;

   private:
    Object(const Object &o);  //コピーコンストラクタによるコピー禁止
    Object &operator=(const Object &o);  //代入によるコピー禁止

    GLuint vao;  //頂点配列オブジェクト(頂点＆色)
    GLuint vbo;  //頂点バッファオブジェクト(頂点or色)
};

/**
 * @brief コンストラクタ
 *
 * @param size 頂点の位置の次元．ここでは２次元
 * @param vertexcount	頂点の数
 * @param vertex	頂点属性を格納した配列
 */
Object::Object(GLuint size, GLsizei vertexcount, const Vertex *vertex) {
    //頂点配列オブジェクト名,1個のvaoの名前が生成され，this->vaoに保存される
    glGenVertexArrays(1, &this->vao);
	//生成したvaoの名前とvaoを結合する
    glBindVertexArray(this->vao);

    //頂点バッファオブジェクト
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex,
		 GL_STATIC_DRAW);

    //結合されている頂点バッファオブジェクトをin変数から参照できるようにする
    // glBindAttribLocation(program, 0,
    // "position")でposition変数に０というインデックスをつけている
    glVertexAttribPointer(0, size, GL_FLAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

Object::~Object() {
    /**
     * @brief vaoオブジェクトの削除
     * @param 1	削除するオブジェクトの数
     * @param this->vao
     */
    glDeleteBuffers(1, &this->vao);

    /**
     * @brief vboオブジェクトの削除
     * @param 1	削除するオブジェクトの数
     * @param this->vbo
     */
    glDeleteBuffers(1, &this->vbo);
}

/**
 * @brief 頂点配列オブジェクトvaoとvaoの名前との結合
 */
void Object::bind() const {
    //描画する頂点配列オブジェクトを指定する
    glBindVertexArray(this->vao);
}
