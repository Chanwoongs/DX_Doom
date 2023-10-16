# DX_Doom
### 'Doom' 이라는 게임의 초기 버전을 레퍼런스로 DirectX 11을 사용하여 구현한 게임입니다.

<div aling="center">
  <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/970d21d6-5fa0-470b-9112-85c65cde94a2" width="854" height="480"/>    
</div>

### [YouTube 영상](https://www.youtube.com/watch?v=kex6V4l4ZNQ&t=56s)
### [DX_Doom 기획서](https://github.com/Chanwoongs/DX_Doom/files/12922610/DX_DOOM.pdf)

### 개요
> #### FPS 게임의 시초를 구현해보고자 시도해본 프로젝트 입니다.
> #### 총을 쏴서 필드내의 좀비를 모두 처치

### 기술
> #### [A* 길찾기 알고리즘 적용](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/astarclass.cpp)
> 지속적인 최적화를 통해 성능 향상 (성능 결과는 YouTube 영상에)
> #### [오브젝트 풀링](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/bulletpoolclass.cpp)
> Bullet의 메모리를 미리 확보하여 사용하고 파괴 시 삭제하지 않고 초기화하여 재사용
> #### [3D 환경에서 2D 몬스터 구현 (UpdateEnemyWalkingAnimation 함수)](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/graphicsclass.cpp)
> Billboard 기법과 선형대수학을 활용해 플레이어가 다방면에서 몬스터를 보더라도 그에 맞는 2D 애니메이션 재생
> #### [FSM을 사용해 적 AI 구현](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/stateMachine.h)
> 외부 FSM 라이브러리를 Import하고 몬스터의 상태들을 정의하여 AI 동작
> #### [HLSL](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/data/light.ps)
> Multi Texturing, Alpha Blending, Normal Mapping, Lighting 적용

### 리팩토링
> 어떤 수학 연산을 할때 XMFLOAT3, XMVECTOR 값을 너무 빈번하게 바꿔, 이를 HelperClass를 만들어 자주 쓰이는 연산을 함수화함
<div>
 <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/b96c0ce3-02f0-40f7-b037-0b36d0a9c6d0" width="350" height="300"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/667c353a-cb3e-4c57-a0ae-1cc9ceda0a36" width="320" height="80"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/14b04662-cfd1-47a2-890a-94007a01157a" width="330" height="17"/>
</div>

> 많은 if, else로 이루어진 조건문을 switch 문으로 가독성을 높이고, Get 함수를 타고 타고 들어가는 구문을 초기에 변수로 선언해 짧게 줄임
<div>
  <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/785b7043-f64b-416a-8133-012a1e59689d" width="430" height="300"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/d1e396f0-85d6-4ca1-a002-f036ac9237bb" width="280" height="350"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/b80c0eea-8ec3-4e3a-ad10-fff6e546a5f6" width="280" height="280"/>
</div>

### 아쉬운 점
> #### 방대한 클래스
> 구현에 집중하다보니 Graphics Class의 크기가 너무 방대해져, 본연의 기능이 아닌 게임 로직의 기능도 도맡고 있다.
> 게임 로직을 전담하는 Game Manager 클래스가 필요해보인다.
> #### Spot Light
> 총을 발사할 때 퍼지는 Spot Lght를 구현하고 싶었지만, 공식이 틀렸는지 제대로 작동하지않고 오히려 엉뚱한 곳에 생겨 사라지지 않았다..
> #### Navmesh
> 적이 움직일 수 있는 범위를 Navmesh와의 충돌로 구현하려 했으나, 이에 대한 이론이 부족하여 구현까지는 하지 못했다.
> 대체로 Text file에 0, 1로 이루어진 맵을 만들어 갈 수 있는 곳을 판별했다.
