#include <iostream>
#include <cstdlib>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(int argc, char const* argv[])
{
	//初期化
	if (glfwInit() == GL_FALSE) {
		std::cerr << "Can't initialize GLFW " << std::endl;
	}

	//ウィンドウ生成
	auto window = std::make_shared<GLFWwindow *const>(glfwCreateWindow(640, 480, "Hello!", nullptr, nullptr));
	if (window == nullptr) {
		std::cerr << "Can't cretate GLFW window." << std::endl;
		return 1;
	}

	//プログラム終了処理を設定する
	std::atexit(glfwTerminate);

	//OpenGL Version 3.2 Core profile を選択する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//複数のウィンドウを扱っているときに，このウィンドウを操作の対象にする
	glfwMakeContextCurrent(*window);

	//GLEWを初期化する
	//GLEWはOpenGLを上書きして，OpenGL1.1以降の新しい機能を提供する
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}

	//垂直同期を使用する
	glfwSwapInterval(1);

	//ウィンドウを白色に塗りつぶす
	glClearColor(1,1,1,0);

	while (glfwWindowShouldClose(*window) == GL_FALSE) {
		//描画内容を消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//ここで描画処理を行う
		
		//カラーバッファを入れ替える
		glfwSwapBuffers(*window);

		//イベントを取り出す
		glfwWaitEvents();
	}
	
	return 0;
}
