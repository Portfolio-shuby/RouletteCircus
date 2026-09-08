# **턴 진행 흐름 관리**
## **Problem**

RouletteCircus의 한 턴은 단순히 플레이어가 한 번 행동하고 끝나는 구조가 아닙니다.
사격 결과에 따라 카드 획득, Keyword 변화, 사망 등의 추가 Gameplay가 발생하며, 모든 결과가 처리된 이후에 다음 플레이어의 턴으로 전환되어야 했습니다.
각 Gameplay Event에서 직접 다음 턴을 시작하도록 구현할 경우, 사격 결과가 완전히 처리되기 전에 턴이 종료되거나 여러 경로에서 Turn End가 중복 호출될 가능성이 있었습니다.

## **Goal**

하나의 턴에서 발생하는 Gameplay Event와 턴 종료 처리를 분리하고, **현재 턴의 모든 결과가 처리된 이후 다음 플레이어로 안전하게 전환되는 Turn Flow를 구현**하는 것을 목표로 하였습니다.

## **Architecture**
[Architecture](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Document/Diagrams/Turn%20Flow%20Management%20Acrchitecture.png)

## **Implementation**
[PlayerGameMode.cpp](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Source/Turn%20Flow%20Management/PlayerGameMode.cpp)

[PlayerTurnController.cpp](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Source/Turn%20Flow%20Management/PlayerTurnController.cpp)

## **Result**

**사격 결과와 Turn End를 분리하여, 하나의 턴에서 발생하는 여러 Gameplay Event가 모두 처리된 이후 다음 턴으로 전환되는 Turn Flow를 구현하였습니다.**

Server_CalculateShootingResults()에서 사격 결과에 따른 카드 및 Keyword를 먼저 정산하고, 이후 EndMyTurn()에서 현재 턴의 상태 초기화, Keyword 처리, Reload, 다음 플레이어 탐색을 수행하도록 역할을 분리하였습니다.

다음 플레이어 탐색 과정에서는 사망한 플레이어를 자동으로 제외하고 Player List를 순환하도록 구현하여, 최대 4명의 플레이어가 참여하는 상황에서도 일관된 Turn Flow를 유지하도록 하였습니다.

최종적으로 개별 Player의 턴 종료 처리는 PlayerTurnController에서 수행하고, 전체적인 Turn 전환은 GameMode에 위임하여 **Gameplay 처리와 전체 게임의 Turn 관리 책임을 분리하였습니다.**
