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

    //�w�b�_�ǂݔ�΂�
    while (fgetc(fp) != '\n');

    while(1) {
        c = fgetc(fp);

        //�����Ȃ烋�[�v�𔲂���B
        if (c == EOF) break;

        //�J���}�����s�łȂ���΁A�����Ƃ��ĂȂ���
        if (c != ',' && c != '\n')
            strcat(buf, (const char*)&c);
        //�J���}�����s�Ȃ烋�[�v������B
        else
            break;

    }

    m_MaxEnemy = atoi(buf);

    //�w�b�_�ǂݔ�΂�
    while (fgetc(fp) != '\n');

    while (1) {

        while (1) {

            c = fgetc(fp);

            //�����Ȃ烋�[�v�𔲂���B
            if (c == EOF)
                goto out;

            //�J���}�����s�łȂ���΁A�����Ƃ��ĂȂ���
            if (c != ',' && c != '\n')
                strcat(buf, (const char*)&c);
            //�J���}�����s�Ȃ烋�[�v������B
            else
                break;
        }
        //�����ɗ����Ƃ������Ƃ́A1�Z�����̕����񂪏o���オ�����Ƃ�������
        switch (col) {
            //1��ڂ͓G��ނ�\���Batoi�֐��Ő��l�Ƃ��đ���B
        case 1:	data[row].type = atoi(buf); break;
            //2��ڂ͒e��ށB�ȍ~�ȗ��B
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
        //�o�b�t�@��������
        memset(buf, 0, sizeof(buf));
        //�񐔂𑫂�
        ++col;

        //�����ǂݍ��񂾕��������s��������񐔂����������čs���𑝂₷
        if (c == '\n') {
            col = 1;
            ++row;
        }
    }
    out:

    //�G�N���X����
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