# Winapi-ArmyWars2 (ArmyWars 2)

WinAPI 기반 C++ 2D 사이드뷰 디펜스/전략 게임 **ArmyWars 2** 프로젝트입니다.  
플레이어와 적 진영이 각자의 유닛과 스킬을 사용해 상대 기지를 파괴하는 것을 목표로 하며, 자체 제작한 간단한 엔진 구조, 상태 머신 기반 유닛 AI, 레벨/스테이지 시스템, UI 매니저 등을 포함합니다.

---

## 링크

- **리소스 다운로드(실행 시 추가필수)**: [`https://drive.google.com/drive/folders/1feToBUGM4sgHUt9uYu9GI3opJgSQLq2s?usp=drive_link`](https://drive.google.com/drive/folders/1feToBUGM4sgHUt9uYu9GI3opJgSQLq2s?usp=drive_link)
- **프로젝트 노션 정리**: [`https://dramatic-mambo-185.notion.site/winapi-2-99bfff27c2d74765b8a6cf547a8032bf?source=copy_link`](https://dramatic-mambo-185.notion.site/winapi-2-99bfff27c2d74765b8a6cf547a8032bf)

---

## 프로젝트 개요

- **장르**: 2D 사이드뷰 디펜스 · 라인전 전략 게임
- **플랫폼**: Windows 데스크톱 (Win32 API)
- **언어**: C++
- **그래픽**: GDI 기반 2D 렌더링 (더블 버퍼링)
- **개발 환경**: Visual Studio, WinAPI

여러 스테이지와 스테이지 선택 화면, 시작 화면, 간단한 에디터 레벨을 통해 전체 흐름이 구성되어 있습니다.

---

## 주요 기능

### 레벨 및 스테이지 시스템

- 추상 `CLevel` 기반의 레벨 구조
- `CLevel_Start`, `CLevel_StgSelect`, `CLevel_Stage01` 등으로 시작/선택/전투 씬 분리
- `LEVEL_TYPE` 열거형으로 스테이지 전환 관리 (`START`, `STAGE_01` ~ `STAGE_04`, `STAGE_SELECT`, `UNIT_SELECT`, `EDITOR` 등)

### 레이어 기반 오브젝트 관리

`LAYER_TYPE`에 따라 오브젝트를 역할별로 분리 관리합니다.

- `BACKGROUND`, `TILE`, `PLATMAP`, `PLATFORM`
- `PLAYER_UNIT`, `PLAYER_BASE`, `PLAYER_UNIT_PROJECTILE`, `PLAYER_UNIT_HEAL`
- `ENEMY_UNIT`, `ENEMY_BASE`, `ENEMY_UNIT_PROJECTILE`, `ENEMY_UNIT_HEAL`
- `PLAYER_SKILL`, `EFFECT`, `MINIMAP`, `UI` 등

### 유닛/스킬 매니저 및 전투 시스템

- `CUnitMgr` 싱글톤으로 유닛/스킬 초기화, 소환 인덱스 관리, 킬 카운트 집계
- 스킬(`SKILL_NAME`)
  - `REPAIR`, `AIRSTRIKE`, `PRECISIONBOOM`
- 유닛 타입/행동/공격 유형 정의
  - `UNIT_TYPE` (`HUMEN`, `MACHINE`, `BASE`)
  - `UNIT_ATST` (`STOP`, `SLOW`, `MOVE`)
  - `UNIT_ATTACK` (`INSTANCE`, `SHOOT`, `HEAL`)

### 상태 머신 기반 유닛 AI

- 추상 상태 `CState` + `CStateMachine` 조합
- 상태별 클래스 예시
  - `DeathState` (사망)
  - `TraceState` (추적/전진)
  - `ShootState` (공격)
- 각 상태는 `Enter / FinalTick / Exit`로 진입/로직/종료를 분리

### UI 및 UX 요소

- `CUIMgr` 싱글톤을 통한 UI 업데이트 및 활성화/차단 프레임 관리
- 버튼/드래그/홀드 UI (`CBtnUI`, `CHoldUI`, `CDragUI` 등)
- 시작 화면(`CLevel_Start`)과 스테이지 선택 화면(`CLevel_StgSelect`) 구성

---

## 기술 스택

- **언어**: C++ (MSVC)
- **플랫폼/라이브러리**: Win32 API, GDI
- **구조/패턴**
  - 싱글톤 매니저 패턴 (`SINGLE` 매크로)
  - 레벨/레이어 기반 씬 관리
  - 상태 머신 기반 AI (`CState` / `CStateMachine`)
---

## 프로젝트 구조 (요약)

- `ArmyWars.sln`: Visual Studio 솔루션
- `ArmyWars/ArmyWars.cpp`: WinAPI 진입점, 메시지 루프, `CEngine::Init`, `CEngine::Progress`
- `ArmyWars/CEngine.*`: 엔진 핵심 (윈도우/HDC/백버퍼/GDI 오브젝트/렌더링)
- `ArmyWars/CLevel.*` 및 `CLevel_*`: 시작/선택/전투 스테이지 레벨들
- `ArmyWars/CUnitMgr.*`: 유닛/스킬 관리 및 소환/집계
- `ArmyWars/CState.*`, `*State.*`: 상태 머신 기반 AI
- `ArmyWars/CAssetMgr.*`, `CTexture.*`: 리소스 관리
- `ArmyWars/CUIMgr.*`, `*UI.*`: UI 시스템

---

## 게임 플레이 개요

- **목표**: 아군 기지를 방어하며 적 유닛/기지를 파괴해 스테이지 클리어
- **유닛**: 보병/저격/정찰/중화기/베테랑/차량/전차 등 역할별 유닛 구성
- **스킬**: 수리, 공습, 정밀 폭격 등 전장 상황을 뒤집는 스킬 제공
- **스테이지**: 여러 전투 스테이지 + 스테이지 선택 화면 제공
