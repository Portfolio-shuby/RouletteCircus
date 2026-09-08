## **Problem**

캐릭터에게 장착 가능한 장신구 종류가 증가하면서 장신구마다 Component를 미리 구성하는 방식은 캐릭터의 Component 구조를 복잡하게 만들고, 새로운 장신구 추가 시 코드 수정이 필요한 문제가 있었습니다.

## **Goal**

장신구를 장착할 때 필요한 Component를 동적으로 생성하고, 장착에 필요한 정보를 DataTable로 분리하여 데이터 추가만으로 새로운 장신구를 적용할 수 있는 확장 가능한 구조를 구현하는 것을 목표로 했습니다.

## **Architecture**
[Architecture](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Document/Diagrams/Accessory%20System%20Architecture.png)

## **Implementation**
[AccessoryComponent.cpp](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Source/Accessory%20System/AccessoryComponent.cpp)

[AccessoryComponent.h](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Source/Accessory%20System/AccessoryComponent.h)

[AccessoryTypes.h](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Source/Accessory%20System/AccessoryTypes.h)

## **DataTable**
[DataTable](https://github.com/Portfolio-shuby/RouletteCircus/blob/main/Document/Screenshots/AccessorySystem%20DataTable.png)

## **Result**

장신구를 장착할 때 필요한 Mesh, Material, Transform 등의 정보를 DataTable에서 가져와 런타임에 Component를 생성하고 캐릭터의 해당 Socket에 Attach하도록 구현하였습니다.

장신구 별 Component를 사전에 구성할 필요가 없으며, 새로운 장신구를 추가할 때 DataTable에 데이터를 추가하는 것만으로 동일한 시스템을 사용할 수 있도록 구성했습니다.

또한 UActorComponent 기반으로 기능을 분리하여 캐릭터의 기존 Logic과 장신구 시스템을 독립적으로 관리할 수 있도록 했습니다.
