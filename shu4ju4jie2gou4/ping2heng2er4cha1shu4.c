#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*#####平衡二叉树#####*/
// 平衡二叉树是一种特殊的二叉排序树

// 二叉树结点
typedef struct ErChaShuJieDian {
    // 结点值
    int iShuZhi;
    // 左子树结点
    struct ErChaShuJieDian *pECSJDZuo;
    // 右子树结点
    struct ErChaShuJieDian *pECSJDYou;
    // 父结点
    struct ErChaShuJieDian *pECSJDFu;
    // 结点深度，下往上依次增加，最下层叶子结点的深度约定为1。
    int iJieDianShenDu;
    // 平衡参数，左右子树深度的差值。如果算出来是[-1,0,1]，就是平衡的。
    int iPingHengCanShu;
} ECSJD;

// 输出数组
extern void shuChuShuZu(int *, int);
// 插入结点
extern void chaRuJieDian(ECSJD **, int, ECSJD **);
//计算结点深度，结点深度=左右子树中深度更大的子树那个子树的深度+1
extern int jiSuanShenDu(ECSJD *);
// 计算平衡参数，结点平衡参数=左子树深度-右子树深度
extern int jiSuanPingHengCanShu(ECSJD *);
// 左旋，自己的右结点变自己的父结点
extern void zuoXuan(ECSJD *, ECSJD **);
// 右旋，自己的左结点变自己的父结点
extern void youXuan(ECSJD *, ECSJD **);
// 中序遍历
extern void zhongXuBianLi(ECSJD *);

int main() {
    // 根节点指针
    ECSJD *pECSJDGen = NULL;
    int iarrDaiPaiXu[10];
    int iarrDaiPaiXuLen = 10;

    srand(time(NULL));
    for (int i = 0; i < iarrDaiPaiXuLen; i++) {
        int chaRuZhi = (rand() % 99) + 1;
        iarrDaiPaiXu[i] = chaRuZhi;
        chaRuJieDian(&pECSJDGen, iarrDaiPaiXu[i], &pECSJDGen);
    }

    printf("dai4pai2xu4:");
    shuChuShuZu(iarrDaiPaiXu, iarrDaiPaiXuLen);

    printf("zhong1xu4bian4li4:");
    zhongXuBianLi(pECSJDGen);
    printf("\n");

    return 0;
}

void shuChuShuZu(int *iarrDaiPaiXu, int iArrDaiPaiXuLen) {
    for (int i = 0; i < iArrDaiPaiXuLen; i++) {
        printf("%d,", iarrDaiPaiXu[i]);
    }
    printf("\n");
}

void chaRuJieDian(ECSJD **tpECSJD, int iChaRuZhi, ECSJD **tpECSJDGen) {
    if (*tpECSJD == NULL) {
        *tpECSJD = (ECSJD *)malloc(sizeof(ECSJD));
        (*tpECSJD)->iShuZhi = iChaRuZhi;
        (*tpECSJD)->pECSJDZuo = NULL;
        (*tpECSJD)->pECSJDYou = NULL;
        (*tpECSJD)->pECSJDFu = NULL;
        (*tpECSJD)->iJieDianShenDu = 1;
        (*tpECSJD)->iPingHengCanShu = 0;
    } else {
        if (iChaRuZhi < (*tpECSJD)->iShuZhi) {
            if ((*tpECSJD)->pECSJDZuo != NULL) {
                chaRuJieDian(&((*tpECSJD)->pECSJDZuo), iChaRuZhi, tpECSJDGen);
            } else {
                (*tpECSJD)->pECSJDZuo = (ECSJD *)malloc(sizeof(ECSJD));
                (*tpECSJD)->pECSJDZuo->iShuZhi = iChaRuZhi;
                (*tpECSJD)->pECSJDZuo->pECSJDZuo = NULL;
                (*tpECSJD)->pECSJDZuo->pECSJDYou = NULL;
                (*tpECSJD)->pECSJDZuo->pECSJDFu = *tpECSJD;
                (*tpECSJD)->pECSJDZuo->iJieDianShenDu = 1;
                (*tpECSJD)->pECSJDZuo->iPingHengCanShu = 0;
            }
        } else if (iChaRuZhi > (*tpECSJD)->iShuZhi) {
            if ((*tpECSJD)->pECSJDYou != NULL) {
                chaRuJieDian(&((*tpECSJD)->pECSJDYou), iChaRuZhi, tpECSJDGen);
            } else {
                (*tpECSJD)->pECSJDYou = (ECSJD *)malloc(sizeof(ECSJD));
                (*tpECSJD)->pECSJDYou->iShuZhi = iChaRuZhi;
                (*tpECSJD)->pECSJDYou->pECSJDZuo = NULL;
                (*tpECSJD)->pECSJDYou->pECSJDYou = NULL;
                (*tpECSJD)->pECSJDYou->pECSJDFu = *tpECSJD;
                (*tpECSJD)->pECSJDYou->iJieDianShenDu = 1;
                (*tpECSJD)->pECSJDYou->iPingHengCanShu = 0;
            }
        } else {
            // 要插入的值已经存在
        }
        // 检查是否需要旋转
        (*tpECSJD)->iPingHengCanShu = jiSuanPingHengCanShu(*tpECSJD);
        if ((*tpECSJD)->iPingHengCanShu > 1) {
            // 左子树高
            if ((*tpECSJD)->pECSJDZuo->iPingHengCanShu == -1) {
                // LR型，先左旋变成LL型
                zuoXuan((*tpECSJD)->pECSJDZuo, tpECSJDGen);
            }
            // LL型
            youXuan(*tpECSJD, tpECSJDGen);
        }
        if ((*tpECSJD)->iPingHengCanShu < -1) {
            // 右子树高，
            if ((*tpECSJD)->pECSJDYou->iPingHengCanShu == 1) {
                // RL型，先右旋变成RR型
                youXuan((*tpECSJD)->pECSJDYou, tpECSJDGen);
            }
            // RR型
            zuoXuan(*tpECSJD, tpECSJDGen);
        }
        // 计算深度和平衡参数
        (*tpECSJD)->iJieDianShenDu = jiSuanShenDu(*tpECSJD);
        (*tpECSJD)->iPingHengCanShu = jiSuanPingHengCanShu(*tpECSJD);
    }
}

int jiSuanShenDu(ECSJD *tpECSJD) {
    int iShenDuZuo = 0, iShenDuYou = 0;
    if (tpECSJD->pECSJDZuo != NULL) {
        iShenDuZuo = tpECSJD->pECSJDZuo->iJieDianShenDu;
    }
    if (tpECSJD->pECSJDYou != NULL) {
        iShenDuYou = tpECSJD->pECSJDYou->iJieDianShenDu;
    }
    return (int)fmax(iShenDuZuo, iShenDuYou) + 1;
}

int jiSuanPingHengCanShu(ECSJD *tpECSJD) {
    int iShenDuZuo = 0, iShenDuYou = 0;
    if (tpECSJD->pECSJDZuo != NULL) {
        iShenDuZuo = tpECSJD->pECSJDZuo->iJieDianShenDu;
    }
    if (tpECSJD->pECSJDYou != NULL) {
        iShenDuYou = tpECSJD->pECSJDYou->iJieDianShenDu;
    }
    return iShenDuZuo - iShenDuYou;
}

void zuoXuan(ECSJD *tpECSJD, ECSJD **tpECSJDGen) {
    // 自己的父结点
    ECSJD *tpECSJDFu = tpECSJD->pECSJDFu;
    // 自己的右结点
    ECSJD *tpECSJDYou = tpECSJD->pECSJDYou;
    // 自己的右结点的左结点
    ECSJD *tpECSJDYouZuo = tpECSJDYou->pECSJDZuo;

    if (tpECSJD == *tpECSJDGen) {
        // 自己是根结点
        *tpECSJDGen = tpECSJDYou;
    }
    // 自己的右结点变成自己的父结点
    tpECSJDYou->pECSJDFu = tpECSJDFu;
    tpECSJDYou->pECSJDZuo = tpECSJD;
    // 自己变成自己的右结点的左结点
    tpECSJD->pECSJDFu = tpECSJDYou;
    if (tpECSJDFu != NULL) {
        // 如果自己有父结点，要把左旋上去的自己的右结点接到父结点相应的位置
        if (tpECSJD == tpECSJDFu->pECSJDZuo) {
            tpECSJDFu->pECSJDZuo = tpECSJDYou;
        } else if (tpECSJD == tpECSJDFu->pECSJDYou) {
            tpECSJDFu->pECSJDYou = tpECSJDYou;
        }
    }
    // 如果自己的右结点有左结点，要把这个左结点变成自己的右结点
    tpECSJD->pECSJDYou = tpECSJDYouZuo;
    if (tpECSJDYouZuo != NULL) {
        tpECSJDYouZuo->pECSJDFu = tpECSJD;
    }
    // 重新计算参与旋转的两个结点的深度和平衡参数
    tpECSJD->iJieDianShenDu = jiSuanShenDu(tpECSJD);
    tpECSJD->iPingHengCanShu = jiSuanPingHengCanShu(tpECSJD);
    tpECSJDYou->iJieDianShenDu = jiSuanShenDu(tpECSJDYou);
    tpECSJDYou->iPingHengCanShu = jiSuanPingHengCanShu(tpECSJDYou);
}

void youXuan(ECSJD *tpECSJD, ECSJD **tpECSJDGen) {
    // 自己的父结点
    ECSJD *tpECSJDFu = tpECSJD->pECSJDFu;
    // 自己的左结点
    ECSJD *tpECSJDZuo = tpECSJD->pECSJDZuo;
    // 自己的左结点的右结点
    ECSJD *tpECSJDZuoYou = tpECSJDZuo->pECSJDYou;

    if (tpECSJD == *tpECSJDGen) {
        *tpECSJDGen = tpECSJDZuo;
    }
    // 自己的左结点变成自己的父结点
    tpECSJDZuo->pECSJDFu = tpECSJDFu;
    tpECSJDZuo->pECSJDYou = tpECSJD;
    // 自己变成自己的左结点的右结点
    tpECSJD->pECSJDFu = tpECSJDZuo;
    if (tpECSJDFu != NULL) {
        // 如果自己有父结点，要把右旋上去的自己的左结点接到父结点相应的位置
        if (tpECSJD == tpECSJDFu->pECSJDZuo) {
            tpECSJDFu->pECSJDZuo = tpECSJDZuo;
        } else if (tpECSJD == tpECSJDFu->pECSJDYou) {
            tpECSJDFu->pECSJDYou = tpECSJDZuo;
        }
    }
    // 如果自己的左结点有右结点，要把这个右结点变成自己的左结点
    tpECSJD->pECSJDZuo = tpECSJDZuoYou;
    if (tpECSJDZuoYou != NULL) {
        tpECSJDZuoYou->pECSJDFu = tpECSJD;
    }
    tpECSJD->iJieDianShenDu = jiSuanShenDu(tpECSJD);
    tpECSJD->iPingHengCanShu = jiSuanPingHengCanShu(tpECSJD);
    tpECSJDZuo->iJieDianShenDu = jiSuanShenDu(tpECSJDZuo);
    tpECSJDZuo->iPingHengCanShu = jiSuanPingHengCanShu(tpECSJDZuo);
}

void zhongXuBianLi(ECSJD *tpECSJD) {
    if (tpECSJD == NULL) {
        return;
    }
    zhongXuBianLi(tpECSJD->pECSJDZuo);
    printf("%d,", tpECSJD->iShuZhi);
    zhongXuBianLi(tpECSJD->pECSJDYou);
}