#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ## 红黑树（R-B TREE） ##
// 红黑树是一种高级的二叉排序树

// ## 二叉树 ##

// 用极值标记无效的结点
#define INT_MIN (1 << 31)
#define INT_MAX (-((1 << 31) + 1))

// 二叉树最大层数
#define TIER_NUM_MAX 8
// 最下层最大结点数
#define TIER_NODE_NUM_MAX (1 << (TIER_NUM_MAX - 1))
// 二叉树最大结点数
#define TREE_NODE_NUM_MAX ((1 << TIER_NUM_MAX) - 1)

#define RED 'r'
#define BLACK 'b'

// 红黑树结点
typedef struct RedBlackTreeNode {
  // 结点值
  int num;
  // 指向左子树的根结点
  struct RedBlackTreeNode *p1Left;
  // 指向右子树的根结点
  struct RedBlackTreeNode *p1Right;
  // 指向父结点
  struct RedBlackTreeNode *p1Prev;
  // 结点颜色，'r'=红色；'b'=黑色
  char color;
} RedBlackTreeNode;

extern void AddNode(RedBlackTreeNode **, int, RedBlackTreeNode *);
void leftRotate(RedBlackTreeNode *, RedBlackTreeNode **, RedBlackTreeNode *);
void rightRotate(RedBlackTreeNode *, RedBlackTreeNode **, RedBlackTreeNode *);
void addNodeFixUp(RedBlackTreeNode *, RedBlackTreeNode **, RedBlackTreeNode *);
extern void DeleteNode(RedBlackTreeNode **, int, RedBlackTreeNode *);
void deleteNodeFixUp(RedBlackTreeNode *, RedBlackTreeNode **, RedBlackTreeNode *);
extern void InorderTraversal(RedBlackTreeNode *, RedBlackTreeNode *);
void doInorderTraversal(RedBlackTreeNode *, RedBlackTreeNode *);
extern int GetDepth(RedBlackTreeNode *);
extern void DrawInConsole(RedBlackTreeNode *);

/**
 * 添加结点
 * @param p2Node 指向二叉树的，指向左子树的根结点的指针或者指向右子树的根结点的指针，的指针
 * @param num
 */
void AddNode(RedBlackTreeNode **p2Root, int num, RedBlackTreeNode *p1NilNode) {
  printf("AddNode: %d\r\n", num);

  // 记录要插入的结点的父结点
  RedBlackTreeNode *p1Prev = NULL;
  // 临时，用于遍历
  RedBlackTreeNode *p1t1Node = NULL;
  // 是否需要插入新结点，默认需要插入
  int isNeedAdd = 1;
  // 临时，存储结点平衡值
  int t1Balance = 0;

  // 没有根结点时，初始化结点
  if (NULL == *p2Root) {
    *p2Root = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
    (*p2Root)->num = num;
    (*p2Root)->p1Left = p1NilNode;
    (*p2Root)->p1Right = p1NilNode;
    (*p2Root)->p1Prev = NULL;
    (*p2Root)->color = BLACK;
    DrawInConsole(*p2Root);
    return;
  }

  // 有结点时，寻找插入插入值的位置

  // 查询的逻辑
  p1Prev = *p2Root;
  p1t1Node = *p2Root;
  while (p1t1Node != NULL && p1t1Node != p1NilNode) {
    if (num < p1t1Node->num) {
      // 插入值小于结点值，去左子树继续查询
      p1Prev = p1t1Node;
      p1t1Node = p1t1Node->p1Left;
    } else if (num > p1t1Node->num) {
      // 插入值大于结点值，去右子树继续查询
      p1Prev = p1t1Node;
      p1t1Node = p1t1Node->p1Right;
    } else {
      // 插入值等于结点值，什么也不做
      isNeedAdd = 0;
      break;
    }
  }

  // 添加结点的逻辑
  if (0 == isNeedAdd) {
    return;
  }
  // 如果需要插入新结点
  // p1Prev 会指向将要插入的结点的父结点，p1t1Node 会指向 NULL
  p1t1Node = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
  p1t1Node->num = num;
  p1t1Node->p1Left = p1NilNode;
  p1t1Node->p1Right = p1NilNode;
  p1t1Node->p1Prev = p1Prev;
  p1t1Node->color = RED;
  // 比较一下结点应该插到哪一边
  if (num < p1Prev->num) {
    p1Prev->p1Left = p1t1Node;
  } else if (num > p1Prev->num) {
    p1Prev->p1Right = p1t1Node;
  }

  DrawInConsole(*p2Root);
  addNodeFixUp(p1t1Node, p2Root, p1NilNode);
}

/**
 * 左旋，和平衡二叉树一个过程
 * @param p1Node 指向要旋转的那个结点
 * @param p2Root 指向，指向根结点的指针，的指针
 */
void leftRotate(RedBlackTreeNode *p1Node, RedBlackTreeNode **p2Root, RedBlackTreeNode *p1NilNode) {
  printf("leftRotate: %d\r\n", p1Node->num);

  RedBlackTreeNode *p1Prev = p1Node->p1Prev;
  RedBlackTreeNode *p1Right = p1Node->p1Right;
  RedBlackTreeNode *p1RightLeft = p1Node->p1Right->p1Left;

  if (p1Node == *p2Root) {
    *p2Root = p1Right;
  }

  p1Node->p1Right = p1NilNode;
  p1Node->p1Prev = p1Right;
  p1Right->p1Left = p1Node;
  p1Right->p1Prev = p1Prev;

  if (p1Prev != NULL) {
    if (p1Node == p1Prev->p1Left) {
      p1Prev->p1Left = p1Right;
    } else if (p1Node == p1Prev->p1Right) {
      p1Prev->p1Right = p1Right;
    }
  }

  p1Node->p1Right = p1RightLeft;
  if (p1RightLeft != p1NilNode) {
    p1RightLeft->p1Prev = p1Node;
  }

  DrawInConsole(*p2Root);
}

/**
 * 右旋，和平衡二叉树一个过程
 * @param p1Node 指向要旋转的那个结点
 * @param p2Root 指向，指向根结点的指针，的指针
 */
void rightRotate(RedBlackTreeNode *p1Node, RedBlackTreeNode **p2Root, RedBlackTreeNode *p1NilNode) {
  printf("rightRotate: %d\r\n", p1Node->num);

  RedBlackTreeNode *p1Prev = p1Node->p1Prev;
  RedBlackTreeNode *p1Left = p1Node->p1Left;
  RedBlackTreeNode *p1LeftRight = p1Node->p1Left->p1Right;

  if (p1Node == *p2Root) {
    *p2Root = p1Left;
  }

  p1Node->p1Left = p1NilNode;
  p1Node->p1Prev = p1Left;
  p1Left->p1Right = p1Node;
  p1Left->p1Prev = p1Prev;

  if (p1Prev != NULL) {
    if (p1Node == p1Prev->p1Left) {
      p1Prev->p1Left = p1Left;
    } else if (p1Node == p1Prev->p1Right) {
      p1Prev->p1Right = p1Left;
    }
  }

  p1Node->p1Right = p1LeftRight;
  if (p1LeftRight != p1NilNode) {
    p1LeftRight->p1Prev = p1Node;
  }

  DrawInConsole(*p2Root);
}

/**
 * 添加结点时的修正
 * @param p1Node 指向要修正的那个结点
 * @param p2Root 指向，指向根结点的指针，的指针
 */
void addNodeFixUp(RedBlackTreeNode *p1Node, RedBlackTreeNode **p2Root, RedBlackTreeNode *p1NilNode) {
  // 叔叔结点（祖父结点的另一个子结点）
  RedBlackTreeNode *p1PrevPrevOtherChild;
  // 根结点不需要调整，父结点为红色时需要调整，如果父结点是黑色的，直接插入就行
  while (p1Node != *p2Root && RED == p1Node->p1Prev->color) {
    if (p1Node->p1Prev == p1Node->p1Prev->p1Prev->p1Left) {
      // 父结点是祖父结点的左子树的根结点，则叔叔结点是祖父结点的右子树的根结点
      p1PrevPrevOtherChild = p1Node->p1Prev->p1Prev->p1Right;
      if (p1PrevPrevOtherChild != NULL && RED == p1PrevPrevOtherChild->color) {
        // 叔叔结点是红色
        // 情况 1：父结点和叔叔结点都是红色
        // 父结点和叔叔结点改黑色，祖父结点改红色
        p1Node->p1Prev->color = BLACK;
        p1PrevPrevOtherChild->color = BLACK;
        p1Node->p1Prev->p1Prev->color = RED;

        printf("addNodeFixUp %d type 1: \r\n", p1Node->num);
        DrawInConsole(*p2Root);

        // 因为祖父结点改了红色，可能破坏红黑树，所以需要继续判断祖父结点是否需要调整
        p1Node = p1Node->p1Prev->p1Prev;
      } else {
        // 叔叔结点是黑色（叔叔结点不存在时，就相当于是定义中的黑色叶子结点）
        if (p1Node == p1Node->p1Prev->p1Right) {
          // 情况 2：叔叔结点是黑色，自己是父结点的右子结点
          // 父结点左旋，由于左旋后父结点变成了自己的左子树，两个红色结点相连一定破坏红黑树，所以继续判断父结点（父结点在更下层）
          // 情况 2 通过这个操作实际上演变成了情况 3

          printf("addNodeFixUp %d type 2: \r\n", p1Node->num);
          leftRotate(p1Node->p1Prev, p2Root, p1NilNode);
          // 把指针指向自己的左子结点（左旋后，父结点变成了自己的左子结点），让场景变成情况 3
          p1Node = p1Node->p1Left;
        } else {
          // 情况 3：叔叔结点是黑色，自己是父结点的左子结点
          // 父结点改黑色，祖父结点改红色，祖父结点右旋
          p1Node->p1Prev->color = BLACK;
          p1Node->p1Prev->p1Prev->color = RED;

          printf("addNodeFixUp %d type 3: \r\n", p1Node->num);
          DrawInConsole(*p2Root);

          rightRotate(p1Node->p1Prev->p1Prev, p2Root, p1NilNode);
        }
      }
    } else {
      // 父结点是祖父结点的右子树的根结点
      // 把上面左子树的操作反过来就行了
      p1PrevPrevOtherChild = p1Node->p1Prev->p1Prev->p1Left;
      if (p1PrevPrevOtherChild != NULL && RED == p1PrevPrevOtherChild->color) {
        // 叔叔结点是红色
        // 情况1：父结点和叔叔结点都是红色
        p1Node->p1Prev->color = BLACK;
        p1PrevPrevOtherChild->color = BLACK;
        p1Node->p1Prev->p1Prev->color = RED;
        p1Node = p1Node->p1Prev->p1Prev;
      } else {
        // 叔叔结点是黑色
        if (p1Node == p1Node->p1Prev->p1Left) {
          // 情况2：叔叔结点是黑色，自己是父结点的左子结点
          rightRotate(p1Node, p2Root, p1NilNode);
          p1Node = p1Node->p1Right;
        } else {
          // 情况3：叔叔结点是黑色，自己是父结点的右子结点
          p1Node->p1Prev->color = BLACK;
          p1Node->p1Prev->p1Prev->color = RED;
          leftRotate(p1Node->p1Prev->p1Prev, p2Root, p1NilNode);
        }
      }
    }
  }
  // 根结点调整为黑色
  (*p2Root)->color = BLACK;
}

/**
 * 删除结点
 * @param p2Root 指向，指向根结点的指针，的指针
 * @param num
 */
void DeleteNode(RedBlackTreeNode **p2Root, int num, RedBlackTreeNode *p1NilNode) {
  printf("DeleteNode: %d\r\n", num);

  // 要删除的结点的父结点
  RedBlackTreeNode *p1Prev = NULL;
  // 要删除的结点
  RedBlackTreeNode *p1Delete = NULL;
  // 临时，用于遍历
  RedBlackTreeNode *p1t1Node = NULL;
  // 临时，用于交换
  int t1Num = 0;

  // 查询的逻辑
  p1t1Node = *p2Root;
  while (p1t1Node != NULL && p1t1Node != p1NilNode) {
    if (num < p1t1Node->num) {
      // 删除值小于结点值，去左子树继续查询
      p1Prev = p1t1Node;
      p1t1Node = p1t1Node->p1Left;
    } else if (num > p1t1Node->num) {
      // 删除值大于结点值，去右子树继续查询
      p1Prev = p1t1Node;
      p1t1Node = p1t1Node->p1Right;
    } else {
      // 删除值等于结点值，要删除的结点（DEL）
      p1Delete = p1t1Node;
      break;
    }
  }

  // 删除结点的逻辑
  if (NULL == p1Delete) {
    return;
  }
  if (p1Delete->p1Left == p1NilNode && p1Delete->p1Right == p1NilNode) {
    //
    if (BLACK == p1Delete->color) {
      deleteNodeFixUp(p1Delete, p2Root, p1NilNode);
    }
    free(p1Delete);
  } else if (p1Delete->p1Left == p1NilNode) {
    // 如果要删除的结点的左子结点为空
    // 用要删除的结点的右子结点，顶替要删除的结点
    // 注意，这里不需要把红黑树的颜色也复制过来，会破坏红黑树的性质
    t1Num = p1Delete->num;
    p1Delete->num = p1Delete->p1Right->num;
    p1Delete->p1Right->num = t1Num;
    if (BLACK == p1Delete->p1Right->color) {
      deleteNodeFixUp(p1Delete->p1Right, p2Root, p1NilNode);
    }
    free(p1Delete->p1Right);
  } else if (p1Delete->p1Right == p1NilNode) {
    // 如果要删除的结点的右子结点为空
    // 用要删除的结点的左子结点，顶替要删除的结点
    // 注意，这里不需要把红黑树的颜色也复制过来，会破坏红黑树的性质
    t1Num = p1Delete->num;
    p1Delete->num = p1Delete->p1Left->num;
    p1Delete->p1Left->num = t1Num;
    if (BLACK == p1Delete->p1Left->color) {
      deleteNodeFixUp(p1Delete->p1Left, p2Root, p1NilNode);
    }
    free(p1Delete->p1Left);
  } else {
    // 如果要删除的结点的左子树和右子树都存在
    // 可以从左子树中找到结点值最大的结点，替代被删除的结点
    // 也可以从右子树中找到结点值最小的结点，替代被删除的结点
    p1Prev = p1Delete;
    p1t1Node = p1Delete->p1Left;
    while (p1t1Node->p1Right != NULL) {
      p1Prev = p1t1Node;
      p1t1Node = p1t1Node->p1Right;
    }
    // 到此 p1t1Node 就是要删除的结点的左子树中结点值最大的结点
    // 直接把左子树中结点值最大的结点的结点值赋值到要删除的结点上
    // 注意，这里不需要把红黑树的颜色也复制过来，会破坏红黑树的性质
    t1Num = p1Delete->num;
    p1Delete->num = p1t1Node->num;
    p1t1Node->num = t1Num;
    if (BLACK == p1t1Node->color) {
      deleteNodeFixUp(p1t1Node, p2Root, p1NilNode);
    }
    // 释放掉结点值最大的结点，这个结点的值已经被记录到原先要删除的结点了
    free(p1t1Node);
  }

  DrawInConsole(*p2Root);
}

/**
 * 删除结点时的修正
 * @param p1Node 指向要删除的那个结点
 * @param p2Root 指向，指向根结点的指针，的指针
 */
void deleteNodeFixUp(RedBlackTreeNode *p1Node, RedBlackTreeNode **p2Root, RedBlackTreeNode *p1NilNode) {
  // 兄弟结点（父结点的另一个子结点）
  RedBlackTreeNode *p1PrevOtherChild;
  // 根结点不需要调整，自己是黑色时需要调整，如果自己是红色的，直接删除就行
  while (p1Node != *p2Root && BLACK == p1Node->color) {
    if (p1Node == p1Node->p1Prev->p1Left) {
      // 自己是父结点的左子结点
      p1PrevOtherChild = p1Node->p1Prev->p1Right;
      if (RED == p1PrevOtherChild->color) {
        // 情况 1：兄弟结点是红色
        // 兄弟结点改为黑色，父结点改为红色，父结点左旋
        p1PrevOtherChild->color = BLACK;
        p1Node->p1Prev->color = RED;
        leftRotate(p1Node->p1Prev, p2Root, p1NilNode);
        // 由于结构发生了变化，需要重新确定兄弟结点
        p1PrevOtherChild = p1Node->p1Prev->p1Right;
      }
      if (BLACK == p1PrevOtherChild->p1Left->color && BLACK == p1PrevOtherChild->p1Right->color) {
        // 情况 2：兄弟结点是黑色，其左子结点是黑色，右子结点是黑色
        // 兄弟结点改为红色，继续判断删除结点的父结点
        p1PrevOtherChild->color = RED;
        p1Node = p1Node->p1Prev;
      }
      if (RED == p1PrevOtherChild->p1Left->color && BLACK == p1PrevOtherChild->p1Right->color) {
        // 情况 3：兄弟结点是黑色，其左子结点是红色，右子结点是黑色
        // 兄弟结点改为红色，兄弟结点的左子结点改为黑色，兄弟结点右旋
        p1PrevOtherChild->p1Left->color = BLACK;
        p1PrevOtherChild->color = RED;
        rightRotate(p1PrevOtherChild, p2Root, p1NilNode);
        // 由于结构发生了变化，需要重新确定兄弟结点
        p1PrevOtherChild = p1Node->p1Prev->p1Right;
      }
      if (RED == p1PrevOtherChild->p1Right->color) {
        // 情况 4：兄弟结点是黑色，其左子结点不管是什么颜色，右子结点是红色
        // 把父结点的颜色赋值给兄弟结点，父结点改为黑色，兄弟结点的右子结点改为黑色，父结点左旋
        p1PrevOtherChild->color = p1Node->p1Prev->color;
        p1Node->p1Prev->color = BLACK;
        p1PrevOtherChild->p1Right->color = BLACK;
        leftRotate(p1Node->p1Prev, p2Root, p1NilNode);
        // 继续判断根结点
        p1Node = *p2Root;
      }
    } else {
      // 自己是父结点的右子结点
      p1PrevOtherChild = p1Node->p1Prev->p1Left;
      if (RED == p1PrevOtherChild->color) {
        // 情况 1：兄弟结点是红色
        // 兄弟结点改为黑色，父结点改为红色，父结点右旋
        p1PrevOtherChild->color = BLACK;
        p1Node->p1Prev->color = RED;
        rightRotate(p1Node->p1Prev, p2Root, p1NilNode);
        // 由于结构发生了变化，需要重新确定兄弟结点
        p1PrevOtherChild = p1Node->p1Prev->p1Left;
      }
      if (BLACK == p1PrevOtherChild->p1Left->color && BLACK == p1PrevOtherChild->p1Right->color) {
        // 情况 2：兄弟结点是黑色，其左子结点是黑色，右子结点是黑色
        // 兄弟结点改为红色，继续判断删除结点的父结点
        p1PrevOtherChild->color = RED;
        p1Node = p1Node->p1Prev;
      }
      if (BLACK == p1PrevOtherChild->p1Left->color && RED == p1PrevOtherChild->p1Right->color) {
        // 情况 3：兄弟结点是黑色，其左子结点是黑色，右子结点是红色
        // 兄弟结点改为红色，兄弟结点的右子结点改为黑色，兄弟结点左旋
        p1PrevOtherChild->p1Right->color = BLACK;
        p1PrevOtherChild->color = RED;
        leftRotate(p1PrevOtherChild, p2Root, p1NilNode);
        // 由于结构发生了变化，需要重新确定兄弟结点
        p1PrevOtherChild = p1Node->p1Prev->p1Left;
      }
      if (RED == p1PrevOtherChild->p1Left->color) {
        // 情况 4：兄弟结点是黑色，其左子结点是红色，右子结点不管是什么颜色
        // 把父结点的颜色赋值给兄弟结点，父结点改为黑色，兄弟结点的左子结点改为黑色，父结点右旋
        p1PrevOtherChild->color = p1Node->p1Prev->color;
        p1Node->p1Prev->color = BLACK;
        p1PrevOtherChild->p1Left->color = BLACK;
        rightRotate(p1Node->p1Prev, p2Root, p1NilNode);
        // 继续判断根结点
        p1Node = *p2Root;
      }
    }
  }
  (*p2Root)->color = BLACK;
}

/**
 * 中序遍历，左根右
 * @param p1Root 指向根结点
 */
void InorderTraversal(RedBlackTreeNode *p1Root, RedBlackTreeNode *p1NilNode) {
  printf("InorderTraversal: ");
  doInorderTraversal(p1Root, p1NilNode);
  printf("\r\n");
}

void doInorderTraversal(RedBlackTreeNode *p1Node, RedBlackTreeNode *p1NilNode) {
  if (NULL == p1Node || p1Node == p1NilNode) {
    return;
  }

  doInorderTraversal(p1Node->p1Left, p1NilNode);
  printf("%d,", p1Node->num);
  doInorderTraversal(p1Node->p1Right, p1NilNode);
}

/**
 * 计算二叉树深度
 * @param p1Node 指向根结点
 * @return
 */
int GetDepth(RedBlackTreeNode *p1Node) {
  int leftDepth, rightDepth;

  if (NULL == p1Node) {
    return 0;
  }

  leftDepth = GetDepth(p1Node->p1Left);
  rightDepth = GetDepth(p1Node->p1Right);

  if (leftDepth >= rightDepth) {
    return leftDepth + 1;
  } else {
    return rightDepth + 1;
  }
}

/**
 * 在控制台输出二叉树，需要借助层次遍历的逻辑
 * @param p1Root 指向根结点
 */
void DrawInConsole(RedBlackTreeNode *p1Root) {
  printf("DrawInConsole: \r\n");

  // 广度优先遍历的队列结构
  RedBlackTreeNode *arr1Queue[TREE_NODE_NUM_MAX];
  int queueHead = 0, queueTail = 0;
  int tierNum = 1, tierTail = 0;
  RedBlackTreeNode *t1Node;

  // 横坐标数据，[0,1=自己在的那个区间的起始坐标和结束坐标，2=父结点的横坐标]
  int arr2XCoordinate[TREE_NODE_NUM_MAX][3];
  // 临时，用于计算横坐标
  int t1MinX = 0;
  // 屏幕数据，[0=自己的结点值，1=自己的纵坐标，2=自己的横坐标，3=父结点的横坐标，4=红黑]
  int arr2PrintData[TREE_NODE_NUM_MAX][5];
  int arr2PrintDataLen = 0;
  // 屏幕缓冲区，坐标上需要存储两个数据，0=结点数值；1=结点颜色
  int arr2PrintBuffer[TIER_NUM_MAX * 4][TIER_NODE_NUM_MAX][2];

  // 初始化
  memset(arr1Queue, 0, sizeof(RedBlackTreeNode *) * TREE_NODE_NUM_MAX);
  memset(arr2XCoordinate, 0, sizeof(int) * TREE_NODE_NUM_MAX * 3);
  memset(arr2PrintData, 0, sizeof(int) * TREE_NODE_NUM_MAX * 4);
  memset(arr2PrintBuffer, 0, sizeof(int) * TIER_NUM_MAX * 4 * TIER_NODE_NUM_MAX * 2);

  arr1Queue[queueTail] = p1Root;
  arr2XCoordinate[queueTail][0] = 0;
  arr2XCoordinate[queueTail][1] = (1 << GetDepth(p1Root)) - 1;
  queueTail++;

  while (queueHead < queueTail) {
    tierTail = queueTail;
    while (queueHead < tierTail) {
      t1Node = arr1Queue[queueHead];
      arr2PrintData[arr2PrintDataLen][0] = t1Node->num;

      // 当前层次遍历到的层数，就是纵坐标
      arr2PrintData[arr2PrintDataLen][1] = tierNum;

      // 计算自己的横坐标（自己在的那个区间的中点坐标）
      t1MinX = (arr2XCoordinate[queueHead][0] + arr2XCoordinate[queueHead][1]) / 2;
      arr2PrintData[arr2PrintDataLen][2] = t1MinX;

      arr2PrintData[arr2PrintDataLen][3] = arr2XCoordinate[queueHead][2];

      arr2PrintData[arr2PrintDataLen][4] = (int)t1Node->color;

      if (t1Node->p1Left != NULL) {
        arr1Queue[queueTail] = t1Node->p1Left;
        arr2XCoordinate[queueTail][0] = arr2XCoordinate[queueHead][0];
        arr2XCoordinate[queueTail][1] = t1MinX - 1;
        arr2XCoordinate[queueTail][2] = t1MinX;
        queueTail++;
      }

      if (t1Node->p1Right != NULL) {
        arr1Queue[queueTail] = t1Node->p1Right;
        arr2XCoordinate[queueTail][0] = t1MinX + 1;
        arr2XCoordinate[queueTail][1] = arr2XCoordinate[queueHead][1];
        arr2XCoordinate[queueTail][2] = t1MinX;
        queueTail++;
      }

      queueHead++;
      arr2PrintDataLen++;
    }
    tierNum++;
  }

  // 用屏幕数据计算屏幕缓冲区的数据
  // 每层结点需要 4 行屏幕缓冲区。第 1 行放置结点值；第 2-4 行放置结点之间的连线。
  for (int i = 0; i < arr2PrintDataLen; i++) {
    // 计算结点值的坐标
    arr2PrintBuffer[(arr2PrintData[i][1] - 1) * 4][arr2PrintData[i][2]][0] = arr2PrintData[i][0];
    arr2PrintBuffer[(arr2PrintData[i][1] - 1) * 4][arr2PrintData[i][2]][1] = arr2PrintData[i][4];
    // 计算和父结点之间的连线怎么画
    if (arr2PrintData[i][2] < arr2PrintData[i][3]) {
      // 结点横坐标在父结点横坐标左边
      // 父结点的正下方，放一个分叉
      arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 1][arr2PrintData[i][3]][0] = -1;
      // 从自己的正上方向右一个位置开始，直到父结点的正下方向左一个位置，填充连线
      for (int index = arr2PrintData[i][2] + 1; index < arr2PrintData[i][3]; index++) {
        arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 2][index][0] = -2;
      }
      // 自己的正上方，放一个分叉
      arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 3][arr2PrintData[i][2]][0] = -3;
    }
    if (arr2PrintData[i][2] > arr2PrintData[i][3]) {
      // 结点横坐标在父结点横坐标右边
      // 把上面的逻辑反过来画就行
      arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 1][arr2PrintData[i][3]][0] = -1;
      for (int index = arr2PrintData[i][2] - 1; index > arr2PrintData[i][3]; index--) {
        arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 2][index][0] = -2;
      }
      arr2PrintBuffer[(arr2PrintData[i][1] - 2) * 4 + 3][arr2PrintData[i][2]][0] = -4;
    }
  }

  // 在控制台输出屏幕缓冲区中的内容
  // 屏幕缓冲区中有效的数据只有 (tierNum - 1) * 4 - 3 行,
  for (int i = 0; i < (tierNum - 1) * 4 - 3; i++) {
    for (int j = 0; j < TIER_NODE_NUM_MAX; j++) {
      if (arr2PrintBuffer[i][j][0] == 0) {
        printf("    ");
      } else if (arr2PrintBuffer[i][j][0] == -1) {
        printf("/  \\");
      } else if (arr2PrintBuffer[i][j][0] == -2) {
        printf("----");
      } else if (arr2PrintBuffer[i][j][0] == -3) {
        printf("  / ");
      } else if (arr2PrintBuffer[i][j][0] == -4) {
        printf(" \\  ");
      } else {
        if (arr2PrintBuffer[i][j][0] != INT_MIN) {
          printf("%03d", arr2PrintBuffer[i][j][0]);
          printf("%c", arr2PrintBuffer[i][j][1]);
        } else {
          printf("nil%c", arr2PrintBuffer[i][j][1]);
        }
      }
    }
    printf("\r\n");
  }
}
