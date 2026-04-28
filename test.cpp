#include <iostream>
#include <cassert>

// 我们把原本写在游戏里的计分逻辑，提炼成一个独立的测试函数
void TestScoringSystem() {
    int score = 0;
    int brickValue = 10;
    bool isBrickHit = true;

    // 模拟球打中了砖块
    if (isBrickHit) {
        score += brickValue;
    }

    // 断言：分数必须变成了 10 分！
    assert(score == 10 && "测试失败：击中砖块后分数没有增加！");
    std::cout << "计分逻辑测试通过！\n";
}

int main() {
    TestScoringSystem();
    // 这里还可以加 TestLifeSystem() 等等
    return 0;
}