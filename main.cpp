#include <Novice.h>
#include<cmath>
#include"MyMath.h"

static const int kWindowWidth = 1280; 
static const int kWindowHeight = 720;

static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y,const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x+kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x+kColumnWidth*2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x+kColumnWidth*3, y, "%s", label);
}

const char kWindowTitle[] = "LE2C_18_トウ_カグン";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraPosition = { 0.0f, 0.0f, -5.0f }; 
	
Vector3 localVertex1 = { 0.0f, -0.5f, 0.0f };
Vector3 localVertex2 = { -0.5f, 0.5f, 0.0f };
Vector3 localVertex3 = { 0.5f, 0.5f, 0.0f };

Vector3 kLocalVertices[3] = { localVertex1, localVertex2, localVertex3 };


	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Vector3 cross = Cross(v1, v2);
    rotate.y += 0.02f;

    if (keys[DIK_A]) {
        translate.x += 0.02f;
    }
    if (keys[DIK_D]) {
        translate.x -= 0.02f;
    }
    if (keys[DIK_W]) {
        translate.z += 0.04f;
    }
    if (keys[DIK_S]) {
        translate.z -= 0.04f;
    }
		Matrix4x4 worldMatrix=MakeAffineMatrix({1.0f,1.0f,1.0f},rotate, translate); 
		Matrix4x4 cameraMatrix= MakeAffineMatrix({1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},cameraPosition); Matrix4x4 viewMatrix = Inverse(cameraMatrix);

Matrix4x4 projectionMatrix=MakePerspectiveFovMatrix(0.45f,float(kWindowWidth)/float(kWindowHeight),0.1f,100.0f);
Matrix4x4 worldViewProjectionMatrix=Multiply(worldMatrix,Multiply(viewMatrix,projectionMatrix));

Matrix4x4 viewportMatrix = MakeViewportMatrix(0,0,float(kWindowWidth),float(kWindowHeight),0.0f, 1.0f); 
Vector3 screenVertices[3];

for (uint32_t i = 0; i < 3; ++i) {

	Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix); 
	screenVertices[i] = Transform(ndcVertex, viewportMatrix);
}
     Vector3 worldVertex1 = Transform(localVertex1, worldMatrix);
        Vector3 worldVertex2 = Transform(localVertex2, worldMatrix);
        Vector3 worldVertex3 = Transform(localVertex3, worldMatrix);

        Vector3 edge1 = { worldVertex2.x - worldVertex1.x, worldVertex2.y - worldVertex1.y, worldVertex2.z - worldVertex1.z };
        Vector3 edge2 = { worldVertex3.x - worldVertex1.x, worldVertex3.y - worldVertex1.y, worldVertex3.z - worldVertex1.z };
        Vector3 normal = Normalize(Cross(edge1, edge2));

        Vector3 cameraToTriangle = { worldVertex1.x - cameraPosition.x, worldVertex1.y - cameraPosition.y, worldVertex1.z - cameraPosition.z };

        float dotProduct = Dot(normal, cameraToTriangle);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrintf(0, 0, cross, "Cross");
   if (dotProduct < 0) { 
            Novice::DrawTriangle(
                int(screenVertices[0].x), int(screenVertices[0].y),
                int(screenVertices[1].x), int(screenVertices[1].y),
                int(screenVertices[2].x), int(screenVertices[2].y),
                RED, kFillModeSolid
            );
        }

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
