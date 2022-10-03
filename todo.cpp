//modify and submit this file only
#include "pa3.h"
#include <iostream>

using namespace std;
//you are NOT allowed to include any additional library; see FAQ

struct TypeStats {
    CarType type;
    int load;
    int maxload;
    int rank;
};

TrainCar *createTrainHead() {
    TrainCar *h = new TrainCar{.type=HEAD, .maxLoad=0, .load=0, .prev=nullptr, .next=nullptr};
    return h;
}

bool addCar(TrainCar *head, int position, CarType type, int maxLoad) {
    if (position <= 0 || type == HEAD || maxLoad <= 0)
        return false;
    TrainCar *p = head;
    for (int i = 0; i < position - 1; ++i) {
        p = p->next;
        if (p == nullptr)
            return false;
    }
    TrainCar *q = new TrainCar{.type=type, .maxLoad=maxLoad, .load=0, .prev=p, .next=p->next};
    if (p->next != nullptr)
        p->next->prev = q;
    p->next = q;
    return true;
}

bool deleteCar(TrainCar *head, int position) {
    if (position < 1)
        return false;
    TrainCar *p = head;
    for (int i = 0; i < position - 1; ++i) {
        p = p->next;
        if (p->next == nullptr)
            return false;
    }
    TrainCar *q = p->next;
    p->next = q->next;
    if (q->next != nullptr)
        q->next->prev = p;
    delete q;
    return true;
}

bool swapCar(TrainCar *head, int a, int b) {
    if (a < 1 || b < 1)
        return false;
    TrainCar *p = head, *q = head;
    for (int i = 0; i < a; ++i) {
        p = p->next;
        if (p == nullptr)
            return false;
    }
    for (int i = 0; i < b; ++i) {
        q = q->next;
        if (q == nullptr)
            return false;
    }
    TrainCar temp{.type=p->type, .maxLoad=p->maxLoad, .load=p->load};
    p->type = q->type;
    p->maxLoad = q->maxLoad;
    p->load = q->load;
    q->type = temp.type;
    q->maxLoad = temp.maxLoad;
    q->load = temp.load;
    return true;
}

void sortTrain(TrainCar *head, bool ascending) {
    TrainCar *p = head->next;
    int lenth = 0;
    while (p != nullptr) {
        p = p->next;
        lenth++;
    }
    if (lenth <= 1)
        return;
    if (ascending) {
        p = head->next->next;
        for (int i = 2; i <= lenth; ++i, p = p->next) {
            TrainCar *q = head->next;
            for (int j = 1; j < i; ++j, q = q->next) {
                if (q->load > p->load) {
                    addCar(head, j, p->type, p->maxLoad);
                    q->prev->load = p->load;
                    q = p;
                    p = p->prev;
                    p->next = q->next;
                    if (q->next != nullptr)
                        q->next->prev = p;
                    delete q;
                    break;
                }
            }
        }
    } else {
        p = head->next->next;
        for (int i = 2; i <= lenth; ++i, p = p->next) {
            TrainCar *q = head->next;
            for (int j = 1; j < i; ++j, q = q->next) {
                if (q->load < p->load) {
                    addCar(head, j, p->type, p->maxLoad);
                    q->prev->load = p->load;
                    q = p;
                    p = p->prev;
                    p->next = q->next;
                    if (q->next != nullptr)
                        q->next->prev = p;
                    delete q;
                    break;
                }
            }
        }
    }
}

bool load(TrainCar *head, CarType type, int amount) {
    TrainCar *p = head->next;
    int r = 0;
    while (p != nullptr) {
        if (p->type == type) {
            r += p->maxLoad - p->load;
        }
        p = p->next;
    }
    if (r < amount)
        return false;
    p = head->next;
    while (amount > 0) {
        if (p->type == type) {
            if ((amount + p->load) <= p->maxLoad) {
                p->load += amount;
                break;
            } else {
                amount -= p->maxLoad - p->load;
                p->load = p->maxLoad;
            }
        }
        p = p->next;
    }
    return true;
}

bool unload(TrainCar *head, CarType type, int amount) {
    TrainCar *p = head->next;
    int r = 0;
    while (p != nullptr) {
        if (p->type == type) {
            r += p->load;
        }
        p = p->next;
    }
    if (r < amount)
        return false;
    p = head;
    while (p->next != nullptr)
        p = p->next;
    while (amount > 0) {
        if (p->type == type) {
            if (p->load >= amount) {
                p->load -= amount;
                break;
            } else {
                amount -= p->load;
                p->load = 0;
            }
        }
        p = p->prev;
    }
    return true;
}

void printCargoStats(const TrainCar *head) {
    TrainCar *p = head->next;
    TypeStats s[5] = {
            {.type=OIL, .load=0, .maxload=0, .rank=0},
            {.type=COAL, .load=0, .maxload=0, .rank=0},
            {.type=WOOD, .load=0, .maxload=0, .rank=0},
            {.type=STEEL, .load=0, .maxload=0, .rank=0},
            {.type=SUGAR, .load=0, .maxload=0, .rank=0}
    };
    int count = 1;
    while (p != nullptr) {
        if (p->type == OIL) {
            s[0].load += p->load;
            s[0].maxload += p->maxLoad;
            if (s[0].rank == 0) {
                s[0].rank = count;
                count++;
            }
        } else if (p->type == COAL) {
            s[1].load += p->load;
            s[1].maxload += p->maxLoad;
            if (s[1].rank == 0) {
                s[1].rank = count;
                count++;
            }
        } else if (p->type == WOOD) {
            s[2].load += p->load;
            s[2].maxload += p->maxLoad;
            if (s[2].rank == 0) {
                s[2].rank = count;
                count++;
            }
        } else if (p->type == STEEL) {
            s[3].load += p->load;
            s[3].maxload += p->maxLoad;
            if (s[3].rank == 0) {
                s[3].rank = count;
                count++;
            }
        } else if (p->type == SUGAR) {
            s[4].load += p->load;
            s[4].maxload += p->maxLoad;
            if (s[4].rank == 0) {
                s[4].rank = count;
                count++;
            }
        }
        p = p->next;
    }
    for (int i = 1; i <= 4; ++i) {
        for (int j = 0; j < 5 - i; ++j) {
            if (s[j].rank > s[j + 1].rank) {
                TypeStats temp = s[j];
                s[j] = s[j + 1];
                s[j + 1] = temp;
            }
        }
    }
    const char enumToStringMapping[6][8] = {"HEAD", "OIL", "COAL", "WOOD", "STEEL", "SUGAR"};
    for (int i = 0; i < 5; ++i) {
        if (s[i].rank == 0)
            continue;
        if (s[i].rank == 1)
            cout << enumToStringMapping[s[i].type] << ":" << s[i].load << "/" << s[i].maxload;
        else
            cout << "," << enumToStringMapping[s[i].type] << ":" << s[i].load << "/" << s[i].maxload;
    }
    cout << endl;
}

void divide(const TrainCar *head, TrainCar *results[CARGO_TYPE_COUNT]) {
    for (int i = 0; i < CARGO_TYPE_COUNT; ++i) {
        results[i] = nullptr;
    }
    TrainCar *p = head->next;
    int index = 0;
    while (p != nullptr) {
        int flag = 0;
        for (int i = 0; i < index; ++i) {
            if (results[i]->next->type == p->type) {
                flag = 1;
                TrainCar *q = results[i];
                while (q->next != nullptr) {
                    q = q->next;
                }
                q->next = new TrainCar{.type=p->type, .maxLoad=p->maxLoad, .load=p->load, .prev=q, .next=nullptr};
            }
        }
        if (flag == 0) {
            results[index] = createTrainHead();
            results[index]->next = new TrainCar{.type=p->type, .maxLoad=p->maxLoad, .load=p->load, .prev=results[index], .next=nullptr};
            index++;
        }
        p = p->next;
    }

}

TrainCar *optimizeForMaximumPossibleCargos(const TrainCar *head, int upperBound) {
    TrainCar *p = head->next;
    int lenth = 0;
    while (p != nullptr) {
        lenth++;
        p = p->next;
    }
    int *load = new int[lenth+1];
    p = head->next;
    int index = 1;
    while (p != nullptr) {
        load[index] = p->load;
        p = p->next;
        index++;
    }
    int **dp = new int *[lenth + 1];
    for (int i = 0; i < lenth + 1; ++i) {
        dp[i] = new int[upperBound + 1];
        dp[i][0] = 0;
    }
    for (int i = 0; i < upperBound + 1; ++i) {
        dp[0][i] = 0;
    }
    for (int i = 1; i <= lenth; ++i) {
        for (int j = 1; j <= upperBound; ++j) {
            if (j < load[i])
                dp[i][j] = dp[i - 1][j];
            else {
                if (dp[i - 1][j] > (dp[i - 1][j - load[i]] + load[i]))
                    dp[i][j] = dp[i - 1][j];
                else
                    dp[i][j] = dp[i - 1][j - load[i]] + load[i];
            }
        }
    }
    int *r = new int[lenth + 1];
    int row = lenth, column = upperBound;
    while (row != 0) {
        if (dp[row][column] == dp[row - 1][column]) {
            r[row] = 0;
            row--;
        } else {
            r[row] = 1;
            column -= load[row];
            row--;
        }
    }
    p = head->next;
    TrainCar *result;
    result = createTrainHead();
    TrainCar *q = result;
    for (int i = 1; i <= lenth; ++i) {
        if (r[i] == 1) {
            q->next = new TrainCar{.type=p->type, .maxLoad=p->maxLoad, .load=p->load, .prev=q, .next=nullptr};
            q = q->next;
        }
        p = p->next;
    }
    for (int i = 0; i < lenth + 1; ++i) {
        delete[]dp[i];
    }
    delete[] dp;
    delete[] r;
    delete[] load;
    return result;
}

void deallocateTrain(TrainCar *head) {
    TrainCar *p = head;
    while (head != nullptr) {
        head = head->next;
        delete p;
        p = head;
    }
}
