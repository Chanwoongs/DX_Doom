# DX_Doom
### 'Doom' 이라는 게임의 초기 버전을 레퍼런스로 DirectX 11을 사용하여 구현한 게임입니다.

<div aling="center">
  <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/970d21d6-5fa0-470b-9112-85c65cde94a2" width="854" height="480"/>    
</div>

### [YouTube 영상](https://www.youtube.com/watch?v=kex6V4l4ZNQ&t=56s)
### [DX_Doom 기획서](https://github.com/Chanwoongs/DX_Doom/files/12922610/DX_DOOM.pdf)

### 개발 기간 (1인 개발)
> #### 2022.10 ~ 2022.12

### 개요
> #### FPS 게임의 시초를 구현해보고자 시도해본 프로젝트 입니다.
> #### 총을 쏴서 필드내의 좀비를 모두 처치

### 기술
> #### [A* 길찾기 알고리즘 적용](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/astarclass.cpp)
> 지속적인 최적화를 통해 성능 향상 (성능 결과는 YouTube 영상에)
> #### [오브젝트 풀링](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/bulletpoolclass.cpp)
> 게임 프로그래밍 패턴 책에서 배운 빈칸 리스트 기법으로 구현. 객체 생성과 동시에 다음 사용할 객체 포인터 위치를 저장하고 소멸 시 삭제가 아닌 초기화
> Bullet의 메모리를 미리 확보하여 사용하고 파괴 시 삭제하지 않고 초기화하여 재사용
> #### [3D 환경에서 2D 몬스터 구현 (UpdateEnemyWalkingAnimation 함수)](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/graphicsclass.cpp)
> Billboard 기법과 선형대수학을 활용해 플레이어가 다방면에서 몬스터를 보더라도 그에 맞는 2D 애니메이션 재생
> #### [FSM을 사용해 적 AI 구현](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/stateMachine.h)
> 외부 FSM 라이브러리를 Import하고 몬스터의 상태들을 정의하여 AI 동작
> #### [HLSL](https://github.com/Chanwoongs/DX_Doom/blob/main/DX_Doom/data/light.ps)
> Multi Texturing, Alpha Blending, Normal Mapping, Environment Mapping , Lighting 적용

### 리팩토링
> 어떤 수학 연산을 할때 XMFLOAT3, XMVECTOR 값을 너무 빈번하게 바꿔, 이를 HelperClass를 만들어 자주 쓰이는 연산을 함수화함
<div>
 <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/b96c0ce3-02f0-40f7-b037-0b36d0a9c6d0" width="350" height="300"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/667c353a-cb3e-4c57-a0ae-1cc9ceda0a36" width="320" height="80"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/14b04662-cfd1-47a2-890a-94007a01157a" width="330" height="17"/>
</div>

> 많은 if, else로 이루어진 조건문을 switch 문으로 가독성을 높이고, Get 함수를 타고 타고 들어가는 구문을 초기에 변수로 선언해 짧게 줄임
<div>
  <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/785b7043-f64b-416a-8133-012a1e59689d" width="430" height="300"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/d1e396f0-85d6-4ca1-a002-f036ac9237bb" width="280" height="350"/> <img src="https://github.com/Chanwoongs/DX_Doom/assets/26241243/b80c0eea-8ec3-4e3a-ad10-fff6e546a5f6" width="280" height="280"/>
</div>

### 프로젝트를 통해 얻은 점
#### 게임 그래픽스 지식
> Geometry, Texturing, Lighting 과 같은 기본 지식과 함께 Multi Texturing, Blending, Environment Mapping 등 고급 그래픽스 기술들의 방법에 대해 알게 되었다.
#### 게임 수학적 지식 
> 원하는 기능을 구현하기 위해 수학을 더욱 공부하고 직접 식을 세워 그리면서 구현하였다. 
#### 객체 생성 및 소멸
> 정말 많은 객체를 포인터로 선언하여 new, delete 연산자로 생성 및 소멸을 시키기 때문에 소멸에 대한 처리가 굉장히 까다로웠다. 생성되지도 않는 객체를 소멸하려고도 하고, 이미 소멸된 객체를 접근하는 등 다양한 에러를 겪었다. 이를 위해 소멸은 모두 Shutdown이란 함수에서 처리하여 디버깅에 용이하도록 하였고, 이미 소멸한 객체는 다시 delete가 호출되지 않도록 처리 하였다.
#### 알고리즘 최적화
> A* 알고리즘을 초기에 List로 구현하였다. 길을 찾을 때 굉장한 프레임 드랍이 있었다. 이를 Vector로 다시 변경하고, 마지막으로 알고리즘 시간에 배운 우선순위 큐로 자료 구조를 변경하였다. 엄청난 프레임 향상이 있었으며, 후처리로 처음부터 갈 수 없는 위치라면 탐색을 하지 않고, 매 프레임마다 길 찾기를 하는게 아닌 좌표가 업데이트될 때 길을 찾는 로직을 추가 했더니 프레임 드랍이 거의 일어나지 않았다.
#### 구현에 대한 자신감
> 처음 게임을 기획했던 의도와 거의 90% 정도 유사하게 구현하였다. 기획 초기엔 아직 모든 지식을 습득하기 전 상태라 긴가민가 하는 마음이었지만, 원했던 게임을 구현한 것 같아 자신감을 많이 얻었다.

### 아쉬운 점
> #### 방대한 클래스
> 구현에 집중하다보니 Graphics Class의 크기가 너무 방대해져, 본연의 기능이 아닌 게임 로직의 기능도 도맡고 있다.
> 게임 로직을 전담하는 Game Manager 클래스가 필요해보인다.
> #### Spot Light
> 총을 발사할 때 퍼지는 Spot Lght를 구현하고 싶었지만, 공식이 틀렸는지 제대로 작동하지않고 오히려 엉뚱한 곳에 생겨 사라지지 않았다..
> #### Navmesh
> 적이 움직일 수 있는 범위를 Navmesh와의 충돌로 구현하려 했으나, 이에 대한 이론이 부족하여 구현까지는 하지 못했다.
> 대체로 Text file에 0, 1로 이루어진 맵을 만들어 갈 수 있는 곳을 판별했다.
> #### Ray Tracing
> 그래픽스에 대해 공부하다보니 Ray Tracing을 너무 적용해보고 싶었다. 그래서 NVIDIA에서 출간한 'RAY TRACING GEMS'라는 748 페이지의 굉장히 두꺼운 책을 도서관에서 빌려 보았지만 아직 이해하고 구현하기엔 너무 어려웠다. 추후엔 사용할 수 있을 만큼의 지식을 얻어 꼭 적용 해보고 싶다.
