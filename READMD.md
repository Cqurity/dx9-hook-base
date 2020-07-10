# dx9-hook-base

이 Repo는 DirectX9 를 기반으로 빌드된 애플리케이션의 Rendering Pipeline 내 주요 API를 Hooking 하여 원하는 동작을 구현합니다.



### Usage

```cpp
// dllMain.cpp

lpWindowCaption = "Target Window Caption Name";
```

타겟 윈도우의 캡션 네임을 입력하여 핸들을 가져옵니다.

키보드 end 키로 메뉴를 키고 끕니다.



`c_render` 클래스의 래핑 메서드로 화면 위에 dx9을 이용하여 원하는 객체를 그릴 수 있습니다.



### External Library

- [Dear ImGui](https://https://github.com/ocornut/imgui)
- [Detours](https://https://github.com/microsoft/Detours)

