# **멀티플레이 동기화**
## **Problem**

RouletteCircus는 최대 4인이 함께 플레이하는 멀티플레이를 제공하기 때문에, 한 플레이어의 행동 결과가 모든 플레이어에게 동일하게 반영되어야 했습니다.
A가 B에게 사격했을 때, 다음과 같은 State가 연쇄적으로 변경됩니다.

- 체력, 총알 수, 사망 여부, 턴 종료 여부, 사격 관련 Event

## **Goal**

Multiplayer 환경에서는 각 Client가 독립적으로 이러한 결과를 계산할 경우 서로 다른 State가 발생할 수 있기 때문에, 사격에 대한 최종 판정을 Listen Server에서 수행하도록 구성하고자 했습니다.
Client에서 발생한 사격 요청을 Server에서 처리하고, Server에서 확정된 결과를 PlayerState의 Replication을 통해 각 Client에 동일하게 반영하는 것을 목표로 했습니다.

## **Architecture**
[Architecture](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Document/Diagrams/Multiplayer%20Synchronization%20Architecture.png)

## **Implementation**


## **Result**

사격 결과에 대한 최종 판정을 Listen Server에서 수행하고, 변경된 PlayerState를 Replication하여 모든 Client가 동일한 Gameplay State를 유지하도록 구현하였습니다.

특히 Life와 같은 주요 플레이어 상태를 PlayerState에서 관리하고 Replicated 변수로 구성하여, Server에서 상태가 변경되면 각 Client에 자동으로 동기화되도록 구현하였습니다.

이를 통해 Client마다 사격 결과를 별도로 계산하지 않고 **Server를 기준으로 일관된 Multiplayer Gameplay State를 유지**할 수 있었습니다.
