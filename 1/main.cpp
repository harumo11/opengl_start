#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//シェーダオブジェクトのコンパイル結果を表示する
//	shader: シェーダオブジェクト名
//	str:	コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
	//コンパイル結果を取得する
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		std::cout << "Compile Error in " << str << std::endl;
	}

	//シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		//シェーダのコンパイル時のログ内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cout << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトのリンク結果を表示する
//	program:	プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program)
{
	//リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::cout << "Link Error" << std::endl;
	}

	//シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cout << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}


//プログラムオブジェクトを作る
//	vsrc: バーテックスシェーダのソースプログラムの文字列
//	fsrc: フラグメントシェーダのソースプログラムの文字列
GLuint createProgram(const char* vsrc, const char *fsrc)
{
	//空のプログラムオブジェクトを作る
	const GLuint program = glCreateProgram();

	if (vsrc != nullptr) {

		//バーテックスシェーダのシェーダオブジェクトを作る
		const GLuint vobj = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vobj, 1, &vsrc, nullptr);
		glCompileShader(vobj);

		//バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(vobj, "vertex shader")) {
			glAttachShader(program, vobj);
		}
		glDeleteShader(vobj);
	}

	if (fsrc != nullptr) {
		//フラグメントシェーダのシェーダオブジェクトを作る
		const GLuint fobj = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fobj, 1, &fsrc, nullptr);
		glCompileShader(fobj);

		//フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(fobj, "vertex shader")) {
			glAttachShader(program, fobj);
		}
		glDeleteShader(fobj);
	}

	//プログラムオブジェクトをリンクする
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	if (printProgramInfoLog(program)) {
		return program;
	}

	//プログラムオブジェクトを作成できなければ０を返す
	glDeleteProgram(program);
	return 0;
}



//シェーダのソースファイルを読み込んだメモリを返す
//	name:	シェーダファイルの名前
//	buffer:	読み込んだソースファイルのテキスト
GLboolean readShaderSource(const char* name, std::vector<GLchar> &buffer)
{
	//ファイル名がnullptrだった
	if (name == nullptr) {
		return false;
	}

	//ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if (file.fail()) {
		//開けなかった
		std::cout << "Error: Can't open source file" << name << std::endl;
		return false;
	}

	//ファイルの末尾に移動し現在位置（＝ファイルサイズ）を得る
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	//ファイルサイズのメモリを確保
	buffer.resize(length+1);


	//ファイルの先頭から読み込む
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail()) {
		//うまく読み込めなかった
		std::cout << "Error: Could not read source file" << name << std::endl;
		file.close();
		return false;
	}

	file.close();
	return true;
}

//シェーダのソースファイルを読み込んでプログラムオブジェクトを作る
//	vert:	バーテックスシェーダのソースファイル名
//	frag:	フラグメントシェーダのソースファイル名
GLuint loadProgram(const char* vert, const char* frag)
{
	//シェーダのソースファイルを読み込む
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	//プログラムオブジェクトを作る
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}


int main(int argc, char const* argv[])
{
	//GLFWを初期化する
	if (glfwInit() == GL_FALSE) {
		//初期化に失敗
		std::cout << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//プログラム終了時の処理を登録する
	std::atexit(glfwTerminate);

	//OpenGL3.2 Core Profileを指定する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//ウィンドウを作る
	GLFWwindow *const window(glfwCreateWindow(640, 640, "Hello!", nullptr, nullptr));
	if (window == nullptr) {
		std::cout << "Can't create window" << std::endl;
		return 1;
	}

	//作ったウィンドウをOpenGLの処理対象にする
	glfwMakeContextCurrent(window);

	//GLEWを初期化する
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	//バッファの入れ替えは垂直同期を使用する
	glfwSwapInterval(0);
	

	//背景色を指定する
	glClearColor(1, 1, 1, 0);

	//プログラムオブジェクトを作る
	const GLuint program = loadProgram("point.vert", "point.frag");

	//ウィンドウが開いている限り繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE) {

		//カラーバッファ（描画用メメモリ）消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//ここで描画処理を行う
		glUseProgram(program);
		
		//カラーバッファ（描画用メモリ）を入れ替える
		glfwSwapBuffers(window);

		//イベントを取り出す
		glfwWaitEvents();
	}
	return 0;
}
