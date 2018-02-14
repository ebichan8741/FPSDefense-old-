#include "main.h"
#include "EnemyManager.h"
#include "enemyShooter.h"
#include "enemy.h"
#include "LifeBar.h"
#include "LifeBarFrame.h"

int CEnemyManager::m_MaxEnemy = 0;

void CEnemyManager::EnemyLoad(void)
{
    FILE *fp;
    ENEMYDATA data[100];
    char buf[100];
    int c;
    int col = 1;
    int row = 0;
    int ShooterID = 0;
    int FighterID = 0;
    int BoomerID = 0;
    int ID = 0;

    memset(buf, 0, sizeof(buf));
    fp = fopen("data/CSV/enemydata.csv", "r");

    //ヘッダ読み飛ばし
    while (fgetc(fp) != '\n');

    while(1) {
        c = fgetc(fp);

        //末尾ならループを抜ける。
        if (c == EOF) break;

        //カンマか改行でなければ、文字としてつなげる
        if (c != ',' && c != '\n')
            strcat(buf, (const char*)&c);
        //カンマか改行ならループ抜ける。
        else
            break;

    }

    m_MaxEnemy = atoi(buf);

    //ヘッダ読み飛ばし
    while (fgetc(fp) != '\n');

    while (1) {

        while (1) {

            c = fgetc(fp);

            //末尾ならループを抜ける。
            if (c == EOF)
                goto out;

            //カンマか改行でなければ、文字としてつなげる
            if (c != ',' && c != '\n')
                strcat(buf, (const char*)&c);
            //カンマか改行ならループ抜ける。
            else
                break;
        }
        //ここに来たということは、1セル分の文字列が出来上がったということ
        switch (col) {
            //1列目は敵種類を表す。atoi関数で数値として代入。
        case 1:	data[row].type = atoi(buf); break;
            //2列目は弾種類。以降省略。
        case 2: data[row].stype = atoi(buf); break;
        case 3: data[row].m_pattern = atoi(buf); break;
        case 4: data[row].s_pattern = atoi(buf); break;
        case 5: data[row].in_time = atoi(buf); break;
        case 6: data[row].shot_time = atoi(buf); break;
        case 7: data[row].pos.x = atoi(buf); break;
        case 8: data[row].pos.y = atoi(buf); break;
        case 9: data[row].pos.z = atoi(buf); break;
        case 10: data[row].speed = atoi(buf); break;
        case 11: data[row].hp = atoi(buf); break;
        }
        //バッファを初期化
        memset(buf, 0, sizeof(buf));
        //列数を足す
        ++col;

        //もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
        if (c == '\n') {
            col = 1;
            ++row;
        }
    }
    out:

    //敵クラス生成
    for (int i = 0; i <= m_MaxEnemy; i++) {
        switch (data[i].type) {
        case ENEMY_TYPE_SHOOTER:
            CGameMode::m_pEnemy.push_back(CEnemyShooter::Create(data[i].pos, data[i].hp, ID));
            //CLifeBarFrame::Create(data[i].pos, ID);
            //CLifeBar::Create(data[i].pos,ID);
            ID++;
            break;

        case ENEMY_TYPE_FIGHTER:
            break;

        case ENEMY_TYPE_BOOMER:
            break;
        }
    }
    fclose(fp);
}

int CEnemyManager::GetMaxEnemy(void)
{
    return m_MaxEnemy;
}