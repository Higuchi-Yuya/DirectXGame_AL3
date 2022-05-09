#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	
	delete model_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	float x_transletion_1 = -10.0f;
	float y_transletion_1 = 10.0f;
	float z_transletion_1 = 0.0f;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				// X,Y,Z方向のスケーリングを設定

				worldTransform_[i][j][k].scale_ = {1.0f, 1.0f, 1.0f};

				/*if (i % 2 == 1 && j % 2 == 1)
				{
					worldTransform_[i][j].scale_ = { 0.0f, 0.0f, 0.0f };
				}*/
				// X,Y,Z軸周りの回転角を設定
				worldTransform_[i][j][k].rotation_ = {XM_PI / 100.5f, 0.0f, 0.0f};

				// X,Y,Z軸周りの平行移動を設定
				worldTransform_[i][j][k].translation_ = {x_transletion_1, y_transletion_1, z_transletion_1};

				//ワールドトランスフォームの初期化
				worldTransform_[i][j][k].Initialize();

				z_transletion_1 += 3;
			}
			z_transletion_1 = 0.0f;
			x_transletion_1 += 3;
		}
		x_transletion_1 = -10.0f;
		y_transletion_1 -= 3;
	}

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

}

void GameScene::Update() { 

////デバッグテキストの表示
//	debugText_->SetPos(50, 50);
//	debugText_->Printf(
//  "translation:(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
//  worldTransform_.translation_.z);
//
//	debugText_->SetPos(50, 70);
//	debugText_->Printf(
//	  "rotation:(%f,%f,%f)", worldTransform_.rotation_.x, worldTransform_.rotation_.y,
//	  worldTransform_.rotation_.z);
//
//	debugText_->SetPos(50, 90);
//	debugText_->Printf(
//	  "scale:(%f,%f,%f)", worldTransform_.scale_.x, worldTransform_.scale_.y,
//	  worldTransform_.scale_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// ----------------------------------------------------------------
	//3Dモデル描画
	for (int i=0;i<9;i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
				model_->Draw(worldTransform_[i][j][k], viewProjection_, textureHandle_);
			}
		}
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
