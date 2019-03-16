#pragma once
#include <iostream>
#include <memory>
#include "Object.hpp"

class Shape {
	public:
		Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex);
		void draw() const;
		virtual void execute() const;

	private:
		std::shared_ptr<const Object> object;

	protected:
		GLsizei vertexcount;
};

/**
 * @brief コンストラクタ
 *
 * @param size	頂点の位置の次元
 * @param vertexcount	頂点の数
 * @param vertex	頂点属性を格納した配列
 */
Shape::Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex){
	//描画する頂点情報を引数から入力
	this->object = std::make_shared<const Object>(size, vertexcount, vertex);
	this->vertexcount = vertexcount;
}

void Shape::draw() const 
{
	//頂点配列オブジェクトvaoをglGenVertexArray()で生成した名前と結合する
	//簡単に言うと描画するvaoの指定
	this->object->bind();

	//描画を実行する
	this->execute();
}

void Shape::execute() const
{
	//折れ線で描画する
	glDrawArrays(GL_LINE_LOOP, 0, this->vertexcount);
}
