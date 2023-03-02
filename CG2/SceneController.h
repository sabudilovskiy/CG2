#pragma once
#include <functional>
#include <unordered_map>
#include <Windows.h>
#include <stdexcept>
#include "WinUtils.h"
#include <windowsx.h>
#include "Scene2D.h"
#include <memory>
namespace cg {
	class SceneController;
	using SceneControllerPtr = std::shared_ptr<SceneController>;
	inline SceneControllerPtr MakeSceneController() {
		return std::make_shared<SceneController>();
	}
	class SceneController {
	protected:
		using MainEvents = UINT;
		using KeyEvents = char;
		using Handle = std::function<void(HWND hWnd, WPARAM wp, LPARAM lp)>;
		template<typename Msg>
		using MapHandlers = std::unordered_map<Msg, Handle>;
		Scene2D scene;
		int color1 = 0;
		int color2 = 0;
		int color3 = 0;
		float weel_max = 2;
		float weel_min = 0.5;
		MapHandlers<MainEvents> main_handlers;
		MapHandlers<KeyEvents> key_handlers;
	protected:
		template<class Msg, class RealMsg>
		void RegisterHandle(MapHandlers<Msg>& map_handlers, RealMsg msg, void (SceneController::* method)()) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(); }
			);
		}
		template<class Msg, class RealMsg>
		void RegisterHandle(MapHandlers<Msg>& map_handlers, RealMsg msg, void (SceneController::* method)(HWND hWnd)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd); }
			);
		}
		template<class Msg, class RealMsg>
		void RegisterHandle(MapHandlers<Msg>& map_handlers, RealMsg msg, void (SceneController::* method)(HWND hWnd, WPARAM wp, LPARAM lp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, wp, lp); }
			);
		}
		template<class Msg, class RealMsg>
		void RegisterHandle(MapHandlers<Msg>& map_handlers, RealMsg msg, void (SceneController::* method)(HWND hWnd, LPARAM lp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, lp); }
			);
		}
		template<class Msg, class RealMsg>
		void RegisterHandle(MapHandlers<Msg>& map_handlers, RealMsg msg, void (SceneController::* method)(HWND hWnd, WPARAM wp)) {
			map_handlers.emplace(msg,
				[this, method](HWND hWnd, WPARAM wp, LPARAM lp) {(this->*method)(hWnd, wp); }
			);
		}
		void DispatchKey(HWND hWnd, WPARAM wp, LPARAM lp) {
			auto it = key_handlers.find(wp);
			if (it != key_handlers.end()) {
				auto& handle = it->second;
				handle(hWnd, wp, lp);
			}
		}
		void BindSystem() {
			RegisterHandle(main_handlers, cg::utils::events::Create , &SceneController::Repaint);
			RegisterHandle(main_handlers, cg::utils::events::Destroy, &SceneController::Destroy);
			RegisterHandle(main_handlers, cg::utils::events::Paint, &SceneController::Repaint);
			RegisterHandle(main_handlers, cg::utils::events::KeyDown, &SceneController::DispatchKey);
		}
		static auto GetMousePoses(LPARAM lp) {
			return std::make_pair(GET_X_LPARAM(lp), GET_Y_LPARAM(lp));
		}
		static auto GetMouseWheel(WPARAM wp) {
			return GET_WHEEL_DELTA_WPARAM(wp);
		}
		void MoveAll(HWND hWnd, Vector2D changes) {
			for (int i = 0; i < scene.size(); i++)
			{
				scene.get_object(i).setPosition(changes);
			}
			StartRepaint(hWnd);
		}
		void PressButtonC(HWND hWnd) {
			ChooseRandomColor();
			StartRepaint(hWnd);
		}
		void ChooseRandomColor() {
			color1 = rand();
			color2 = rand();
			color3 = rand();
		}
		void StartRepaint(HWND hWnd) {
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
		}
		void Repaint(HWND hWnd) {
			PAINTSTRUCT ps;
			auto hdc = BeginPaint(hWnd, &ps); // начало отрисовки
			auto hBrush = CreateSolidBrush(RGB(color1, color2, color3)); // создание кисточки
			SelectObject(hdc, hBrush); // выбор кисточки
			scene.draw(hdc, hWnd);
			EndPaint(hWnd, &ps);
		}
		void ChangeSize(HWND hWnd, WPARAM wp) {
			for (int i = 0; i < scene.size(); i++)
			{
				float wheelNow = GetMouseWheel(wp);
				auto [x1, y1] = scene.get_object(0).m_vertices[0];
				Vector2D vec(-x1, -y1);
				scene.get_object(i).setPosition(vec);
				if (wheelNow > 0)
				{
					scene.get_object(i).setScale(weel_max);
				}
				else
				{
					scene.get_object(i).setScale(weel_min);
				}
				Vector2D vec1(x1, y1);
				scene.get_object(i).setPosition(vec1);

			}
			StartRepaint(hWnd);
		}
		void Destroy() {
			PostQuitMessage(0);
		}
		void MoveLeft(HWND hWnd) {
			MoveAll(hWnd, {-100, 0 });
		}
		void MoveRight(HWND hWnd) {
			MoveAll(hWnd, { 100, 0 });
		}
		void MoveDown(HWND hWnd) {
			MoveAll(hWnd, { 0, 100 });
		}
		void MoveUp(HWND hWnd) {
			MoveAll(hWnd, { 0, -100 });
		}
		void InverseY(HWND hWnd) {
			for (int i = 0; i < scene.size(); i++)
			{
				auto [x1, y1] = scene.get_object(0).m_vertices[0];
				Vector2D vec(-x1, -y1);
				auto& object = scene.get_object(i);
				object.setPosition(vec);
				object.setRotation(3.14 / 2);
				object.Mirror(0);
				object.setRotation(-3.14 / 2);
				Vector2D vec1(x1, y1);
				object.setPosition(vec1);
			}
			StartRepaint(hWnd);
		}
		void InverseX(HWND hWnd) {
			for (int i = 0; i < scene.size(); i++)
			{
				auto [x1, y1] = scene.get_object(0).m_vertices[0];
				auto& object = scene.get_object(i);
				Vector2D vec(-x1, -y1);
				object.setPosition(vec);
				object.setRotation(-3.14 / 2);
				object.Mirror(1);
				object.setRotation(3.14 / 2);
				Vector2D vec1(x1, y1);
				object.setPosition(vec1);
			}
			StartRepaint(hWnd);
		}
		void LeftRotation(HWND hWnd) {
			for (int i = 0; i < scene.size(); i++)
			{
				auto [x1, y1] = scene.get_object(0).m_vertices[0];
				auto& object = scene.get_object(i);
				Vector2D vec(-x1, -y1);
				object.setPosition(vec);
				object.setRotation(3.14 / 4);
				Vector2D vec1(x1, y1);
				object.setPosition(vec1);
			}
			StartRepaint(hWnd); 
		}
		void RightRotation(HWND hWnd) {
			for (int i = 0; i < scene.size(); i++)
			{
				auto [x1, y1] = scene.get_object(0).m_vertices[0];
				auto& object = scene.get_object(i);
				Vector2D vec(-x1, -y1);
				object.setPosition(vec);
				object.setRotation(-3.14 / 4);
				Vector2D vec1(x1, y1);
				object.setPosition(vec1);
			}
			StartRepaint(hWnd);
		}
	public:
		SceneController(
			float weel_max = 2,
			const float weel_min = 0.5,
			Scene2D scene = {}) : scene(std::move(scene)), weel_max(weel_max), weel_min(weel_min)
		{
			BindSystem();
			RegisterHandle(main_handlers, cg::utils::events::MouseWheel, &SceneController::ChangeSize);
			RegisterHandle(key_handlers, cg::utils::Keys::C, &SceneController::PressButtonC);
			RegisterHandle(key_handlers, cg::utils::Keys::ArrowLeft, &SceneController::MoveLeft);
			RegisterHandle(key_handlers, cg::utils::Keys::ArrowRight, &SceneController::MoveRight);
			RegisterHandle(key_handlers, cg::utils::Keys::ArrowDown, &SceneController::MoveDown);
			RegisterHandle(key_handlers, cg::utils::Keys::ArrowUp, &SceneController::MoveUp);
			RegisterHandle(key_handlers, cg::utils::Keys::Q, &SceneController::LeftRotation);
			RegisterHandle(key_handlers, cg::utils::Keys::E, &SceneController::RightRotation);
			RegisterHandle(key_handlers, cg::utils::Keys::W, &SceneController::InverseX);
			RegisterHandle(key_handlers, cg::utils::Keys::R, &SceneController::InverseY);
			ChooseRandomColor();
		}
		
		void SetScene(Scene2D scene_) { 
			scene = std::move(scene_); 
		}
		bool Dispatch(HWND hWnd, MainEvents msg, WPARAM wp, LPARAM lp) {
			auto it = main_handlers.find(msg);
			if (it != main_handlers.end()) {
				auto& handle = it->second;
				handle(hWnd, wp, lp);
				return true;
			}
			return false;
		}
	};
}
