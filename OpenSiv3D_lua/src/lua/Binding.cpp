#include "Binding.hpp"
#include "NamespaceSetter.hpp"
#include "GlobalScript.hpp"
#include "Bind/PointBind.hpp"
#include "Bind/Vec2Bind.h"
#include <Siv3D.hpp>

void s3d::Lua::Binding::All() {
  SystemBind();
  WindowBind();
  ShapeBind();
  FontBind();
  DrawableTextBind();
  GraphicBind();
  CursorBind();
  KeyBind();
  MouseBind();
  TextureBind();
}


void s3d::Lua::Binding::SystemBind() {
  GlobalScript
    .setNamespace(L"System")
    .setFunction(L"Update", sol::overload(System::Update, []() {return System::Update(); }))
    .setFunction(L"Exit", &System::Exit)
    .setFunction(L"SetExitEvent", &System::SetExitEvent)
    .setFunction(L"GetPreviousEvent", &System::GetPreviousEvent)
    .setFunction(L"DeltaTime", sol::overload(System::DeltaTime, []() {return System::DeltaTime(); }))
    .setFunction(L"Sleep", sol::overload(sol::resolve<void(int32)>(&System::Sleep), sol::resolve<void(const MillisecondsF&)>(&System::Sleep)))
    .setFunction(L"FrameCount", &System::FrameCount)
    .setFunction(L"SetFrameCount", &System::SetFrameCount)
    .setFunction(L"LaunchBrowser", &System::LaunchBrowser)
    .setFunction(L"ShowLicenses", &System::ShowLicenses);
}

void s3d::Lua::Binding::WindowBind() {
  GlobalScript
    .setNamespace(L"ShowState")
    .setValue(L"Normal", ShowState::Normal)
    .setValue(L"Minimized", ShowState::Minimized)
    .setValue(L"Maximized", ShowState::Maximized);


  GlobalScript
    .setClass<WindowState>(L"WindowState")
    .value(L"clientSize", &WindowState::clientSize)
    .value(L"windowSize", &WindowState::windowSize)
    .value(L"pos", &WindowState::pos)
    .value(L"frameSize", &WindowState::frameSize)
    .value(L"titleBarHeight", &WindowState::titleBarHeight)
    .value(L"title", &WindowState::title)
    .value(L"showState", &WindowState::showState)
    .value(L"focused", &WindowState::focused)
    .value(L"fullScreen", &WindowState::fullScreen);

  GlobalScript
    .setNamespace(L"Window")
    .setValue(L"DefaultClientSize", sol::readonly_property(Window::DefaultClientSize))
    .setFunction(L"SetTitle", sol::resolve<void(const String&)>(&Window::SetTitle))
    .setFunction(L"GetState", &Window::GetState)
    .setFunction(L"Size", &Window::Size)
    .setFunction(L"Center", &Window::Center)
    .setFunction(L"Width", &Window::Width)
    .setFunction(L"Height", &Window::Height)
    .setFunction(L"ClientRect", &Window::ClientRect)
    .setFunction(L"SetPos", &Window::SetPos)
    .setFunction(L"SetPos", sol::overload(sol::resolve<bool(int32, int32, bool)>(&Window::Resize),
      [](int32 w, int32 h) {return Window::Resize(w, h); },
      sol::resolve<bool(const s3d::Size&, bool)>(&Window::Resize),
      [](const s3d::Size& s) {return Window::Resize(s); }))
    .setFunction(L"SetBaseSize", &Window::SetBaseSize)
    .setFunction(L"BaseSize", &Window::BaseSize)
    .setFunction(L"BaseCenter", &Window::BaseCenter)
    .setFunction(L"BaseWidth", &Window::BaseWidth)
    .setFunction(L"BaseHeight", &Window::BaseHeight)
    .setFunction(L"BaseClientRect", &Window::BaseClientRect);
}

void s3d::Lua::Binding::FontBind() {
  Lua::GlobalScript
    .setClass<Font>(L"Font")
    .constructor<Font(), Font(int32, Typeface, FontStyle), Font(int32), Font(int32, Typeface)>()
    .metaFunction(sol::meta_function::call, (DrawableText(Font::*)(const String&)const)(&Font::operator()));
}

void s3d::Lua::Binding::DrawableTextBind() {
  Lua::GlobalScript
    .setClass<DrawableText>(L"DrawableText")
    .constructor<DrawableText(const DrawableText&), DrawableText(DrawableText&&)>()
    .function(L"draw", sol::overload(sol::resolve<RectF(double, double, const ColorF&)const>(&DrawableText::draw),
      [](DrawableText& obj, double x, double y, const Color& c) {return obj.draw(x, y, c); },
      sol::resolve<RectF(const Vec2&, const ColorF&)const>(&DrawableText::draw),
      [](DrawableText& obj, const Vec2& v, const Color& c) {return obj.draw(v, c); },
      [](DrawableText& obj, const Point& v, const ColorF& c) {return obj.draw(v, c); },
      [](DrawableText& obj, const Point& v, const Color& c) {return obj.draw(v, c); }))
    .function(L"drawAt", sol::overload(sol::resolve<RectF(double, double, const ColorF&)const>(&DrawableText::drawAt),
      [](DrawableText& obj, double x, double y, const Color& c) {return obj.drawAt(x, y, c); },
      sol::resolve<RectF(const Vec2&, const ColorF&)const>(&DrawableText::drawAt),
      [](DrawableText& obj, const Vec2& v, const Color& c) {return obj.drawAt(v, c); },
      [](DrawableText& obj, const Point& v, const ColorF& c) {return obj.drawAt(v, c); },
      [](DrawableText& obj, const Point& v, const Color& c) {return obj.drawAt(v, c); }));
}

void s3d::Lua::Binding::GraphicBind() {
  GlobalScript
    .setNamespace(L"Graphics")
    .setFunction(L"SetBackground", &Graphics::SetBackground);
}

void s3d::Lua::Binding::CursorBind() {
  GlobalScript
    .setNamespace(L"Cursor")
    .setFunction(L"Pos", &Cursor::Pos);
}

void s3d::Lua::Binding::MouseBind() {
  GlobalScript
    .setValue(L"MouseL", &MouseL);
}

void s3d::Lua::Binding::KeyBind() {
  GlobalScript
    .setClass<Key>(L"Key")
    .function(L"pressed", &Key::pressed);
}

void s3d::Lua::Binding::TextureBind() {
  GlobalScript
    .setClass<Emoji>(L"Emoji")
    .constructor<Emoji(const String&)>();

  GlobalScript
    .setClass<Texture>(L"Texture")
    .constructor<Texture(const Emoji&, TextureDesc)>()
    .function(L"resize",sol::resolve<TextureRegion(double)const>(&Texture::resize));

  GlobalScript
    .setClass<TextureRegion>(L"TextureRegion")
    .constructor<TextureRegion()>()
    .function(L"draw", [](TextureRegion& obj, double x, double y) {return obj.draw(x, y); });

  GlobalScript
    .setNamespace(L"TextureDesc")
    .setValue(L"Mipped", TextureDesc::Mipped);
}

void s3d::Lua::Binding::ShapeBind() {
  PointBind();
  Vec2Bind();
}
