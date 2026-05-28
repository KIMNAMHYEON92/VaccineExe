# VACCINE.exe — 시스템 방어 프로토콜

> **Unreal Engine 5.7 C++ 기반의자체 발광(Emissive) 테마 1인 개발 탑뷰 아케이드 슈팅 게임**
>
> 본 프로젝트는 외부 완성형 그래픽 에셋 소싱에 소모되는 리소스를 최소화하고, 언리얼 엔진 5의 C++ 코어 시스템 프레임워크 제어 및 런타임 최적화 성능을 검증하기 위해 '린(Lean) 개발' 프로세스로 구축된 독립형 프로젝트입니다.

<br>

## 🎮 플레이 데모 및 핵심 메커니즘
| 🚀 런타임 무기 자동 성장 (Rapid/Multi-Shot) | 🛡️ 수학적 Clamp 공간 통제 및 카이팅 조작 |
| :---: | :---: |
| ![Weapon Evolution](https://i.imgur.com/C9WWOtn.gif) | ![Space Control](https://i.imgur.com/U6ohdbj.gif) |

<br>

## 📄 핵심 문서 및 실행 파일 링크
* **[상세 기술서]** [프로젝트 진행 보고서 v1 (Google Docs)](https://docs.google.com/document/d/1VCnnZ37Y72J8GXcGspiz0Sg4_V7wMLEAmvW7tzRnpiQ/edit?usp=sharing)
* **[실행 파일]** [Win64 독립형 패키징 릴리즈 빌드 다운로드](https://drive.google.com/file/d/1dqGlpgJgK-c_pLwwU9C8vReYuMjjji24/view?usp=sharing)

<br>

## 📌 제품 아키텍처 및 핵심 명세 (Specification)
* **장르:** 탑뷰 아케이드 로그라이트 생존 슈팅 (Vampire Survivors-like)
* **개발 기간:** 2026.05.11 ~ 2026.05.21 (8일, 1인 개발 완성)
* **타겟 플랫폼:** PC Windows (독립형 실행 파일 패키징 완료)
* **아트 톤앤매너:** '컴퓨터 격리 구역의 해킹 공간'이라는 래트로 사이버펑크 설정을 위해 3D 외부 메쉬를 전면 배제하고, 기본 도형(`Cube`, `Sphere`)과 고강도 `Emissive Material`, 엔진 내 `Post Processing (Bloom)` 연산만을 조합하여 고효율 비주얼 구축.

<br>

## 🛠️ 기술 스택 및 개발 환경 (Tech Stack)
* **Engine:** Unreal Engine 5.7 (C++ 프레임워크 기반, 구조적 제어 목적)
* **Language:** C++17
* **Framework Components:** `APlayerPawn`, `ABullet`, `AEnemyActor`, `AEnemyFactory`, `AKillZone`
* **Input System:** Enhanced Input (C++ 기반 `Axis2D` 매핑 및 모디파이어 제어)
* **Build Tool:** Unreal Build Tool (UBT) 모듈 종속성 설정 (`EnhancedInput` 주입)
* **IDE & VCS:** JetBrains Rider / Git & GitHub

<br>

## 💎 핵심 기능 및 구현 프레임워크 (Core Features)

### 1. Enhanced Input 동적 연동 및 트윈 스틱 자동 조준 조작선
* 최신 `Enhanced Input` 아키텍처를 도입, `Axis2D` 입력의 상하좌우 수학적 방향 벡터(`MoveDirection`)를 실시간 연산하도록 구현.
* 단순 입력 처리에 그치지 않고, 플레이어의 이동 벡터가 존재할 시 총구를 담당하는 `UArrowComponent`의 월드 회전값(`SetWorldRotation`)을 `MoveDirection.Rotation()`으로 즉각 동기화하는 C++ 로직 설계.
* **결과:** 마우스 조준 피로도를 원천 차단하고 도망치며 전방위로 사격하는 **'카이팅(Kiting) 플레이'**의 직관적 UX 완성.

### 2. 난수 분기형 AI 행동 트리 및 동적 무작위 웨이브 스포너
* 고정 위치 적층 스폰 버그를 방지하기 위해 `AEnemyFactory` 생성 좌표에 `FMath::RandRange(-500.f, 500.f)`의 2D 랜덤 오프셋을 연산하여 무작위 스폰 공간 구현.
* 스폰된 `AEnemyActor`는 생성 시점에 1~100 사이의 난수를 발생시켜, 고유 확률 변수(`traceRate: 50%`) 이내 도달 시 `Tick`마다 플레이어를 실시간 추격하는 **'유도 추적형'**, 범위 이탈 시 초기 방향으로 직진하는 **'돌격형'**으로 행동 아키텍처가 동적 분기되도록 설계.

### 3. 임계점 검사 기반 실시간 발사체 자동 성장(Scaling) 구조
* 게임 전체 상태를 관장하는 `AVaccineGameModeBase`와 플레이어 폰의 `FTimerHandle` 자동 발사 타이머 구조를 긴밀히 결합.
* 글로벌 점수 누적 함수 내부에서 실시간 조건문 검사(Threshold Check)를 수행하여, **20점 돌파 시 발사 주기를 초고속 연사 모드(0.5s -> 0.2s)로 마개조**하고, **50점 돌파 시 좌우 15도 공간 오프셋 각도를 적용한 3갈래 산탄(Multi-Shot) 발사** 메커니즘으로 실시간 자동 진화하는 런타임 스케일링 성장 루프 완성.

<br>

## 🔧 핵심 기술적 도전 및 트러블슈팅 (Troubleshooting)

### 1. 고속 이동 액터 간 프레임 관통(터널링) 및 Z축 이탈 완벽 제어
* **문제 상황:** 탑뷰 2.5D 환경에서 3D 물리 연산 수행 중 소수점 오차로 인해 `ABullet`과 `AEnemyActor` 간의 Z축(높이) 데이터 불일치 및 고속 이동 시 충돌 이벤트가 무시되고 관통하는 터널링 현상 발견.
* **해결 방식:** 1. 무거운 3D 입체 연산 비용을 통제하기 위해 모든 상호작용 액터의 `Tick` 최하단 위치 보정부에서 **`NewLocation.Z = 0.0f;` 코드를 꽂아 넣어 높이 좌표를 물리적으로 강제 고정**.
  2. `SetActorLocation` 함수 실행 시 `bSweep = true` 파라미터를 인젝션하여 이동 경로 상의 오버랩을 전수 감지하도록 변경하고, 각 박스 컴포넌트의 **CCD(Continuous Collision Detection)** 옵션을 활성화함.
* **결과:** 판정 누수율 0% 달성, 시스템 신뢰도 확보.

### 2. 물리 볼륨 한계 돌파를 위한 수학적 경계선 가두리 기법 최적화
* **문제 상황:** 화면 이탈 방지를 위해 `Blocking Volume` 바리케이드를 쳤으나, 고속 가속도 진입 시 플레이어가 벽을 뚫고 나가거나 물리 엔진 꼬임으로 벽면에 끼여 영구 고립되는 예외 버그 발생.
* **해결 방식:** 불필요한 물리 엔진 충돌 차단(`Block`) 연산 레이어를 맵에서 전부 삭제하고, 프로그래머가 좌표를 수학적으로 완전 지배하는 **`FMath::Clamp` 함수 알고리즘 기법**을 `Tick` 함수에 배치.

```cpp
  CurrentLocation.X = FMath::Clamp(CurrentLocation.X, -600.0f, 600.0f);
  CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, -1000.0f, 1000.0f);
  SetActorLocation(CurrentLocation, true);
```

* **결과:** 무거운 물리 연산 비용 제거로 **엔진 최적화 마진 확보**, 어떠한 가속도에서도 절대 화면을 탈출할 수 없는 100% 신뢰도의 수학적 펜스 구축.

### 3. 제품 릴리즈 안정성을 위한 리스크 관리와 스펙-아웃(Spec-out) 결단

* **문제 상황:** 고강도 `Emissive Material (강도 50.0)` 및 포스트 프로세스의 `Bloom` 효과가 화면을 채우자, 엔진 내 자동 노출(Auto Exposure) 장치가 눈부심을 막기 위해 화면 전체 노출값을 최하단으로 압착함. 이로 인해 반투명 성격의 나이아가라 파티클 시스템이 암전 영역에 파묻혀 식별되지 않는 그래픽 파이프라인 충돌 확인.
* **해결 방식:** 셰이더 및 포스트 프로세싱 렌더 레이어 전면 수정 시 마감 데드라인 내 빌드 불안정성 및 크래시 리스크가 고조됨을 계산함. 1인 개발 창업가 관점에서 불완전한 이펙트에 매몰되는 것보다 "단 하나의 예외 크래시도 없는 완벽하게 안정적인 마스터 패키징 빌드를 데드라인 내에 시장(평가장)에 릴리즈하는 것"이 최우선 핵심 가치라고 판단, 파티클 연산 코드를 과감히 **스펙-아웃(Spec-out)** 처리하고 사운드 시스템 플레이 환경을 극대화하는 방향으로 동적 사양 타협 수행.
* **결과:** 마감 시한 전 단 한 번의 빌드 실패 없이 **Windows 64비트 독립형 실행 폴더 패키징 릴리즈에 완벽 성공.** 해당 그래픽 파이프라인 매커니즘 충돌 건은 향후 과제(Future Work) 기술 명세서로 이관하여 리스크 관리 문서화 완료.

## 📎 보고서 및 향후 과제 (Future Works)

* 본 프로젝트의 상세한 아키텍처 설계 배경, 컴파일 설정 모듈화 과정 및 일자별 상세 디버깅 기록은 상단 첨부된 [프로젝트 진행 보고서 v1]에서 투명하게 확인하실 수 있습니다.
* **Next Steps:** 1. 데이터 주도(Data-Driven) 설계를 위한 외부 CSV 시트 및 `UDataTable` 구조 도입 연동.
2. 포스트 프로세스 수동 노출(Manual Exposure) 밸런싱을 통한 나이아가라 네온 파티클 가시성 확보 및 그래픽 파이프라인 최적화.
