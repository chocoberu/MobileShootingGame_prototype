# MobileShootingGame_prototype

목표  : UE4, C++를 이용해서 모바일 탑다운 슈팅 게임 프로토타입 구현 테스트

- 싱글 플레이, 멀티 플레이, 연습 모드 플레이 가능
- 싱글 플레이는 제한 시간 내에 몬스터를 쓰러트리는 것이 목표
- OnlineSubSystemNull을 이용해서 멀티 플레이 구현, 이후에 Epic Online Service로 교체 예정
- 멀티 플레이는 로비에서 세션 생성 or 입장한 후 모든 플레이어가 준비가 됐을 때 게임 시작 가능
- 멀티 플레이는 제한 시간 동안 상대 팀을 더 많이 처치하는 것이 목표


![화면 캡처 2022-05-10 180012](https://user-images.githubusercontent.com/43817454/187429954-b9cfc79c-f728-4dd0-aba9-87978751ea3d.png)
게임 시작 화면

![화면 캡처 2022-05-11 132124](https://user-images.githubusercontent.com/43817454/187429996-f33ea3a4-e081-4e70-895f-4dd081f7d7a7.png)
멀티 플레이 로비 화면

![Screenshot_20220830-165005_MobileShooting](https://user-images.githubusercontent.com/43817454/187430129-e690c75a-2f48-4dd2-bbe9-32f0e0299803.jpg)
멀티 플레이 세션 화면