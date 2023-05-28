
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 20
#define ROLL_SIZE 15
#define SUBJECT_SIZE 5

typedef enum
{
    FIRST,
    SECOND,
    THIRD,
    FOURTH
} YEAR;
typedef enum
{
    FALSE,
    TRUE
} BOOLEAN;
typedef enum
{
    PENDING,
    CLEAR
} FEE_STATUS;
typedef enum
{
    NOTAPPLIED,
    APPLIED
} APPLICANT;

typedef struct Attendance_List_Tag AttendanceList;
typedef struct FeeStatus_List_Tag FeeStatusList;
typedef struct ApplicantStatus_List_Tag ApplicantStatusList;

typedef struct Student_Data_Tag
{
    char name[NAME_SIZE];
    // DEPARTMENT dep;
    char dep[5];
    YEAR year;
    char rollno[ROLL_SIZE];
    int classesConducted[5];
    int classesAttended[5];
    FEE_STATUS fee;
    APPLICANT app;
} DATA;

typedef DATA Item_Type;

typedef struct General_Record_Student_List_Tag
{
    Item_Type student_Data;

    struct General_Record_Student_List_Tag *next;

} GR_StudentList;

struct Attendance_List_Tag
{
    char rollno[ROLL_SIZE];
    // DEPARTMENT dep;
    char dep[5];
    float attendance_Percent;
    struct Attendance_List_Tag *next;
};

struct FeeStatus_List_Tag
{
    char roll[ROLL_SIZE];
    FEE_STATUS fee;
    struct FeeStatus_List_Tag *next;
};

struct ApplicantStatus_List_Tag
{
    char name[NAME_SIZE];
    char roll[ROLL_SIZE];
    APPLICANT apply;
    struct ApplicantStatus_List_Tag *next;
};

int MCC[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};
int MCA[4][5] =
    {
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1}};

//

//

//

//

//                      General_Record_Student_List UTILITY FUNCTIONS ARE DEFINED HERE

//

//
void filling_MCC(Item_Type data)
{
    int row = data.year;
    int col = -1;
    char c = data.dep[0];
    switch (c)
    {
    case 'C': //CS or CV
        if (data.dep[1] == 'S')
            col = 0;
        else
            col = 1;
        break;
    case 'E':
        if (data.dep[1] == 'C')
            col = 2;
        else
            col = 3;
        break;
    case 'M':
        col = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    if (col != -1 && MCC[row][col] == -1) // TABULATION
    {
        int maxclasses_conducted = data.classesConducted[0];

        for (int i = 1; i < 5; i++) //no of subjects
        {
            if (maxclasses_conducted < data.classesConducted[i])
            {
                maxclasses_conducted = data.classesConducted[i];
            }
        }
        MCC[row][col] = maxclasses_conducted;
    }
    //filling MCA for MCC[row][col] 
    for (int i = 1; i < 5; i++) //no of subjects
    {
        if (MCC[row][col] == data.classesConducted[i])
        {
            if (MCA[row][col] < data.classesAttended[i])
            {
                // printf("HELLO");
                MCA[row][col] = data.classesAttended[i];
            }
        }
    }
}

GR_StudentList *InsertAtStartGRSL(Item_Type d, GR_StudentList *head)
{
    GR_StudentList *nptr;
    nptr = (GR_StudentList *)malloc(sizeof(GR_StudentList));
    if (nptr == NULL)
    {
        printf("\nMEMORY NOT AVAILABLE");
    }
    else
    {
        nptr->student_Data = d;
        nptr->next = head;
        head = nptr;
    }
    return head;
}

void readStudent(DATA *s, FILE *fp)
{
    fscanf(fp, "%s %s %d %s", s->name, s->dep, &s->year, s->rollno);
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%d", &s->classesConducted[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%d", &s->classesAttended[i]);
    }
    fscanf(fp, "%d %d", &s->fee, &s->app);
}
GR_StudentList *inputFromFile(char *filename, DATA student, GR_StudentList *head)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp))
    {
        readStudent(&student, fp);
        student.year = student.year - 1;// to use ENUM logic
        filling_MCC(student);
        head = InsertAtStartGRSL(student, head);
    }

    fclose(fp);
    return head;
}

GR_StudentList *takeInput_General_Student_Record_List()
{
    GR_StudentList *head = NULL;

    DATA arr;
    head = inputFromFile("sample.txt", arr, head);
    return head;
}


void printGSRL(GR_StudentList *head)
{
    GR_StudentList *t = head;
    while (t)
    {
        printf("\tYEAR= %d", t->student_Data.year + 1);
        printf("\t %s", t->student_Data.dep);
        printf("\t %20s", t->student_Data.name);
        printf("\t %s", t->student_Data.rollno);
        printf("\tFEE= %d", t->student_Data.fee);
        printf("\tAPPLY= %d\n", t->student_Data.app);

        t = t->next;
    }
    printf("\n");
}
GR_StudentList *DeleteAtStartGRSL(Item_Type *eptr, GR_StudentList *lptr)
{
    GR_StudentList *nptr = lptr;
    *eptr = nptr->student_Data;
    lptr = lptr->next;
    free(nptr);
    nptr = NULL;
    return lptr;
}
void DeleteGSRL(GR_StudentList *lptr)
{
    Item_Type data;
    while (lptr)
    {
        lptr = DeleteAtStartGRSL(&data, lptr);
        printf("\tYEAR= %d", data.year + 1);
        printf("\t %s", data.dep);
        printf("\t %20s", data.name);
        printf("\t %s", data.rollno);
        printf("\tFEE= %d", data.fee);
        printf("\tAPPLY= %d\n", data.app);
    }
}


GR_StudentList *mergeGSRL(GR_StudentList *lptr1, GR_StudentList *lptr2)
{
    GR_StudentList *result, *tail, *ptr1, *ptr2;
    ptr1 = lptr1;
    ptr2 = lptr2;

    if (ptr1->student_Data.year < ptr2->student_Data.year)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else if (ptr1->student_Data.year > ptr2->student_Data.year)
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }
    else //Same Sem ,SORT Department wise Lexograpphically
    {
        if (strcmp(ptr1->student_Data.dep, ptr2->student_Data.dep) < 0)
        {
            result = ptr1;
            ptr1 = ptr1->next;
        }
        else if (strcmp(ptr1->student_Data.dep, ptr2->student_Data.dep) > 0)
        {
            result = ptr2;
            ptr2 = ptr2->next;
        }
        else // Dep also same ,sort based on roll no
        {
            if (strcmp(ptr1->student_Data.rollno, ptr2->student_Data.rollno) < 0)
            {
                result = ptr1;
                ptr1 = ptr1->next;
            }
            else
            {
                result = ptr2;
                ptr2 = ptr2->next;
            }
        }
    }

    tail = result;

    while (ptr1 && ptr2)
    {
        if (ptr1->student_Data.year < ptr2->student_Data.year)
        {
            tail->next = ptr1;
            tail = tail->next;
            ptr1 = ptr1->next;
        }
        else if (ptr1->student_Data.year > ptr2->student_Data.year)
        {
            tail->next = ptr2;
            tail = tail->next;
            ptr2 = ptr2->next;
        }
        else //Same Sem ,SORT Department wise Lexograpphically
        {
            if (strcmp(ptr1->student_Data.dep, ptr2->student_Data.dep) < 0)
            {
                tail->next = ptr1;
                tail = tail->next;
                ptr1 = ptr1->next;
            }
            else if (strcmp(ptr1->student_Data.dep, ptr2->student_Data.dep) > 0)
            {
                tail->next = ptr2;
                tail = tail->next;
                ptr2 = ptr2->next;
            }
            else // Dep also same ,sort based on roll no
            {
                if (strcmp(ptr1->student_Data.rollno, ptr2->student_Data.rollno) < 0)
                {
                    tail->next = ptr1;
                    tail = tail->next;
                    ptr1 = ptr1->next;
                }
                else
                {
                    tail->next = ptr2;
                    tail = tail->next;
                    ptr2 = ptr2->next;
                }
            }
        }
    }
    if (ptr1)
        tail->next = ptr1;
    else
        tail->next = ptr2;
    return result;
}
GR_StudentList *DivideGSRL(GR_StudentList *lptr)
{
    GR_StudentList *nptr, *fast, *slow;
    slow = lptr;
    fast = lptr->next->next;
    while (fast)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast)
            fast = fast->next;
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}
GR_StudentList *MergeSortGSRL(GR_StudentList *head)
{
    GR_StudentList *nptr, *lptr;
    lptr = head;
    if (lptr && lptr->next)
    {
        nptr = DivideGSRL(head);
        lptr = MergeSortGSRL(lptr);
        nptr = MergeSortGSRL(nptr);
        lptr = mergeGSRL(lptr, nptr);
    }
    return lptr;
}

//

//

//

//

//                  CREATING UTUILITY FUNCTIONS FOR ATTENDANCE LIST

//

void initializeAttendanceList(AttendanceList **AttendanceListArray)
{
    for (int i = 0; i < 4; i++)
        AttendanceListArray[i] = NULL;
}
AttendanceList *InsertAtStartARL(char *roll, char *dep, float attendance_Percent, AttendanceList *head)
{
    AttendanceList *nptr;
    nptr = (AttendanceList *)malloc(sizeof(AttendanceList));
    if (nptr == NULL)
    {
        printf("\nMEMORY NOT AVAILABLE");
    }
    else
    {
        nptr->attendance_Percent = attendance_Percent;
        strcpy(nptr->rollno, roll);
        strcpy(nptr->dep, dep);
        nptr->next = head;
        head = nptr;
    }
    return head;
}

void CreateAttendanceList(AttendanceList *AttendanceListArray[4], GR_StudentList *head_Of_GSRL)
{
    GR_StudentList *ptr = head_Of_GSRL;
    while (ptr)
    {
        int yr = ptr->student_Data.year;
        float att_percent = -1;

        int row = ptr->student_Data.year;
        int col = -1;
        char c = ptr->student_Data.dep[0];
        switch (c)
        {
        case 'C': //CS or CV
            if (ptr->student_Data.dep[1] == 'S')
                col = 0;
            else
                col = 1;
            break;
        case 'E':
            if (ptr->student_Data.dep[1] == 'C')
                col = 2;
            else
                col = 3;
            break;
        case 'M':
            col = 4;
            break;
        default:
            printf("\nWrong character is %c", c);
            printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
            break;
        }
        for (int i = 0; i < SUBJECT_SIZE; i++)
        {
            if (MCC[row][col] == ptr->student_Data.classesConducted[i])
            {
                float att_For_Current_Subject = (ptr->student_Data.classesAttended[i] * 1.0) / (MCA[row][col]) * 100.0;

                if (att_percent < att_For_Current_Subject && att_percent <= 100)
                {
                    att_percent = att_For_Current_Subject;
                }
                if (att_percent <= 0 || att_percent > 100)
                {
                    printf("\nroll=%s  ptr->student_Data.classesAttended[i]=%d   MCA[row][col]=%d \n", ptr->student_Data.rollno, ptr->student_Data.classesAttended[i], MCA[row][col]);
                }
            }
        }

        AttendanceListArray[yr] = InsertAtStartARL(ptr->student_Data.rollno, ptr->student_Data.dep, att_percent, AttendanceListArray[yr]);
        ptr = ptr->next;
    }
}

void printARL(AttendanceList *head)
{
    if (!head)
    {
        printf("\nEMPTY ATTENDANCE LIST\n");
        return;
    }

    AttendanceList *t = head;
    while (t)
    {
        printf("\t %s", t->rollno);
        printf("\t %s", t->dep);
        printf("\tAttendance%%= %f \n", t->attendance_Percent);

        t = t->next;
    }
    printf("\n");
}
void printAllARL(AttendanceList *AttendanceListArray[4])
{
    for (int i = 0; i < 4; i++)
    {
        printf("\nATTENDANCE LIST FOR YEAR %d\n", i + 1);
        printARL(AttendanceListArray[i]);
    }
}
AttendanceList *DeleteAtStartARL(AttendanceList *eptr, AttendanceList *lptr)
{
    AttendanceList *nptr = lptr;
    eptr->attendance_Percent = nptr->attendance_Percent;
    strcpy(eptr->rollno, nptr->rollno);
    strcpy(eptr->dep, nptr->dep);

    lptr = lptr->next;
    free(nptr);
    nptr = NULL;
    return lptr;
}
void DeleteARL(AttendanceList *lptr)
{
    if (!lptr)
    {
        printf("\nEMPTY ATTENDANCE LIST\n");
        return;
    }

    AttendanceList data;
    while (lptr)
    {
        lptr = DeleteAtStartARL(&data, lptr);
        printf("\t %s", data.rollno);
        printf("\t %s", data.dep);
        printf("\tATTEDNDANCE %%= %f \n", data.attendance_Percent);
    }
}
void DeleteAllARL(AttendanceList *AttendanceListArray[4])
{
    for (int i = 0; i < 4; i++)
    {
        printf("\nYEAR %d\n", i + 1);
        DeleteARL(AttendanceListArray[i]);
    }
}

AttendanceList *mergeARL(AttendanceList *lptr1, AttendanceList *lptr2)
{
    AttendanceList *result, *tail, *ptr1, *ptr2;
    ptr1 = lptr1;
    ptr2 = lptr2;

    if (strcmp(ptr1->rollno, ptr2->rollno) < 0)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }

    tail = result;

    while (ptr1 && ptr2)
    {
        if (strcmp(ptr1->rollno, ptr2->rollno) < 0)
        {
            tail->next = ptr1;
            tail = tail->next;
            ptr1 = ptr1->next;
        }
        else
        {
            tail->next = ptr2;
            tail = tail->next;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1)
        tail->next = ptr1;
    else
        tail->next = ptr2;
    return result;
}
AttendanceList *DivideARL(AttendanceList *lptr)
{
    AttendanceList *nptr, *fast, *slow;
    slow = lptr;
    fast = lptr->next->next;
    while (fast)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast)
            fast = fast->next;
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}
AttendanceList *MergeSortARL(AttendanceList *head)
{
    AttendanceList *nptr, *lptr;
    lptr = head;
    if (lptr && lptr->next)
    {
        nptr = DivideARL(head);
        lptr = MergeSortARL(lptr);
        nptr = MergeSortARL(nptr);
        lptr = mergeARL(lptr, nptr);
    }
    return lptr;
}
void MergeSortAllARL(AttendanceList *AttendanceListArray[4])
{
    for (int i = 0; i < 4; i++)
        AttendanceListArray[i] = MergeSortARL(AttendanceListArray[i]);
}

//

//

//                  CREATING UTUILITY FUNCTIONS FOR FEE STATUS LIST

//
FeeStatusList *InsertAtStartFSL(FEE_STATUS fee, char *roll, FeeStatusList *head)
{
    FeeStatusList *nptr;
    nptr = (FeeStatusList *)malloc(sizeof(FeeStatusList));
    if (nptr == NULL)
    {
        printf("\nMEMORY NOT AVAILABLE");
    }
    else
    {
        nptr->fee = fee;
        strcpy(nptr->roll, roll);
        nptr->next = head;
        head = nptr;
    }
    return head;
}
FeeStatusList *CreateFeeStatusList(FeeStatusList *head_Of_FSL, GR_StudentList *head_Of_GSRL)
{
    FeeStatusList data;
    while (head_Of_GSRL)
    {
        head_Of_FSL = InsertAtStartFSL(head_Of_GSRL->student_Data.fee, head_Of_GSRL->student_Data.rollno, head_Of_FSL);
        head_Of_GSRL = head_Of_GSRL->next;
    }
    return head_Of_FSL;
}

void printFSL(FeeStatusList *head)
{
    FeeStatusList *t = head;
    while (t)
    {
        printf("\t %s", t->roll);
        printf("\tFEE STATUS= %d \n", t->fee);

        t = t->next;
    }
    printf("\n");
}

FeeStatusList *DeleteAtStartFSL(FeeStatusList *eptr, FeeStatusList *lptr)
{
    FeeStatusList *nptr = lptr;
    eptr->fee = nptr->fee;
    strcpy(eptr->roll, nptr->roll);
    lptr = lptr->next;
    free(nptr);
    nptr = NULL;
    return lptr;
}
void DeleteFSL(FeeStatusList *lptr)
{
    FeeStatusList data;
    while (lptr)
    {
        lptr = DeleteAtStartFSL(&data, lptr);
        printf("\t %s", data.roll);
        printf("\tFEE STATUS= %d \n", data.fee);
    }
}

FeeStatusList *mergeFSL(FeeStatusList *lptr1, FeeStatusList *lptr2)
{
    FeeStatusList *result, *tail, *ptr1, *ptr2;
    ptr1 = lptr1;
    ptr2 = lptr2;

    if (strcmp(ptr1->roll, ptr2->roll) < 0)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }

    tail = result;

    while (ptr1 && ptr2)
    {
        if (strcmp(ptr1->roll, ptr2->roll) < 0)
        {
            tail->next = ptr1;
            tail = tail->next;
            ptr1 = ptr1->next;
        }
        else
        {
            tail->next = ptr2;
            tail = tail->next;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1)
        tail->next = ptr1;
    else
        tail->next = ptr2;
    return result;
}
FeeStatusList *DivideFSL(FeeStatusList *lptr)
{
    FeeStatusList *nptr, *fast, *slow;
    slow = lptr;
    fast = lptr->next->next;
    while (fast)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast)
            fast = fast->next;
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}
FeeStatusList *MergeSortFSL(FeeStatusList *head)
{
    FeeStatusList *nptr, *lptr;
    lptr = head;
    if (lptr && lptr->next)
    {
        nptr = DivideFSL(head);
        lptr = MergeSortFSL(lptr);
        nptr = MergeSortFSL(nptr);
        lptr = mergeFSL(lptr, nptr);
    }
    return lptr;
}

//

//

//

//
//                  CREATING UTUILITY FUNCTIONS FOR APPLICANT STATUS LIST
//
ApplicantStatusList *InsertAtStartASL(char *name, char *roll, APPLICANT appl, ApplicantStatusList *head)
{
    ApplicantStatusList *nptr;
    nptr = (ApplicantStatusList *)malloc(sizeof(ApplicantStatusList));
    if (nptr == NULL)
    {
        printf("\nMEMORY NOT AVAILABLE");
    }
    else
    {
        nptr->apply = appl;
        strcpy(nptr->roll, roll);
        strcpy(nptr->name, name);
        nptr->next = head;
        head = nptr;
    }
    return head;
}
ApplicantStatusList *CreateApplicantStatusList(ApplicantStatusList *head_Of_ASL, GR_StudentList *head_Of_GSRL)
{
    FeeStatusList data;
    while (head_Of_GSRL)
    {
        head_Of_ASL = InsertAtStartASL(head_Of_GSRL->student_Data.name, head_Of_GSRL->student_Data.rollno, head_Of_GSRL->student_Data.app, head_Of_ASL);
        head_Of_GSRL = head_Of_GSRL->next;
    }
    return head_Of_ASL;
}

void printASL(ApplicantStatusList *head)
{
    ApplicantStatusList *t = head;
    while (t)
    {
        printf("\t %20s", t->name);
        printf("\t %s", t->roll);
        printf("\tAPPLICANT STATUS= %d\n", t->apply);

        t = t->next;
    }
    printf("\n");
}

ApplicantStatusList *DeleteAtStartASL(ApplicantStatusList *eptr, ApplicantStatusList *lptr)
{
    ApplicantStatusList *nptr = lptr;
    strcpy(eptr->name, nptr->name);
    strcpy(eptr->roll, nptr->roll);
    eptr->apply = nptr->apply;
    strcpy(eptr->roll, nptr->roll);
    lptr = lptr->next;
    free(nptr);
    nptr = NULL;
    return lptr;
}
void DeleteASL(ApplicantStatusList *lptr)
{
    ApplicantStatusList data;
    while (lptr)
    {
        lptr = DeleteAtStartASL(&data, lptr);
        printf("\t %20s", data.name);
        printf("\t %s", data.roll);
        printf("\tAPPLICANT STATUS= %d \n", data.apply);
    }
}

ApplicantStatusList *mergeASL(ApplicantStatusList *lptr1, ApplicantStatusList *lptr2)
{
    ApplicantStatusList *result, *tail, *ptr1, *ptr2;
    ptr1 = lptr1;
    ptr2 = lptr2;

    if (strcmp(ptr1->roll, ptr2->roll) < 0)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }

    tail = result;

    while (ptr1 && ptr2)
    {
        if (strcmp(ptr1->roll, ptr2->roll) < 0)
        {
            tail->next = ptr1;
            tail = tail->next;
            ptr1 = ptr1->next;
        }
        else
        {
            tail->next = ptr2;
            tail = tail->next;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1)
        tail->next = ptr1;
    else
        tail->next = ptr2;
    return result;
}
ApplicantStatusList *DivideASL(ApplicantStatusList *lptr)
{
    ApplicantStatusList *nptr, *fast, *slow;
    slow = lptr;
    fast = lptr->next->next;
    while (fast)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast)
            fast = fast->next;
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}
ApplicantStatusList *MergeSortASL(ApplicantStatusList *head)
{
    ApplicantStatusList *nptr, *lptr;
    lptr = head;
    if (lptr && lptr->next)
    {
        nptr = DivideASL(head);
        lptr = MergeSortASL(lptr);
        nptr = MergeSortASL(nptr);
        lptr = mergeASL(lptr, nptr);
    }
    return lptr;
}

//

//

//

//
void Question1(GR_StudentList *head);
void Question2(ApplicantStatusList *head);
FeeStatusList *Question3(AttendanceList *AttendanceListArray[4], ApplicantStatusList *head_Of_ASL, FeeStatusList *head_Of_FSL);
AttendanceList *Question4(AttendanceList *AttendanceListArray[4]);
void Question5(FeeStatusList *head_Of_FSL, AttendanceList *AttendanceListArray[4]);
FeeStatusList *Question6(AttendanceList *list_Of_Low_Attendance, FeeStatusList *head_Of_FSL);
//

//

//
int main()
{
    GR_StudentList *head_Of_GSRL = NULL;

    //TAKES INPUT THROUGH FILE HANDLING AND RETURNS FIRST NODE OF THE GSR_LIST
    head_Of_GSRL = takeInput_General_Student_Record_List();
    //PRINTING 1ST LIST
    printf("\nGSRL\n");
    printGSRL(head_Of_GSRL);

    head_Of_GSRL = MergeSortGSRL(head_Of_GSRL);
    printf("\nGSRL(sorted)\n");

    printGSRL(head_Of_GSRL);
    //NOW 1ST LIST-> GSRL IS CREATED

    //

    //

    //

    //

    //

    //
    //NOW WE CREATE 4 ATTENDANCE LISTS

    AttendanceList *AttendanceListArray[4];
    initializeAttendanceList(AttendanceListArray);
    for (int i = 0; i < 4; i++)
    {
        if (AttendanceListArray[i] == NULL)
        {
            printf("INIT success\n");
        }
    }
    CreateAttendanceList(AttendanceListArray, head_Of_GSRL);
    printf("2nd LIST(ALL)(ATTENDANCE)\n");
    printAllARL(AttendanceListArray);
    printf("2nd LIST(ALL)(ATTENDANCE)(Sorted)\n");
    MergeSortAllARL(AttendanceListArray);
    printAllARL(AttendanceListArray);
    //

    //

    //

    //

    //

    // NOW WE CREATE 3RD LIST -> FEE STATUS LIST

    FeeStatusList *head_Of_FSL = NULL;
    head_Of_FSL = CreateFeeStatusList(head_Of_FSL, head_Of_GSRL);
    printf("\nPRINTING 3rd LIST(FEE STATUS)\n");
    printFSL(head_Of_FSL);
    head_Of_FSL = MergeSortFSL(head_Of_FSL);
    printf("\nPRINTING 3rd LIST(FEE STATUS)(sorted)\n");
    printFSL(head_Of_FSL);
    //

    //

    //

    // NOW WE CREATE 4TH LIST -> APPLICANT STATUS LIST
    ApplicantStatusList *head_Of_ASL = NULL;
    head_Of_ASL = CreateApplicantStatusList(head_Of_ASL, head_Of_GSRL);
    printf("\nPRINTING 4th LIST(APPLICANT)\n");
    printASL(head_Of_ASL);
    head_Of_ASL = MergeSortASL(head_Of_ASL);
    printf("\nPRINTING 4th LIST(APPLICANT)(sorted)\n");
    printASL(head_Of_ASL);
    //
    //

    //

    //

    //

    //
    //                          SOLVING THE QUESTIONS

    printf("\nEXECUTING QUESTION 1\n");
    Question1(head_Of_GSRL);
    //

    printf("\nEXECUTING QUESTION 2\n");
    Question2(head_Of_ASL);
    //
    //

    printf("\nEXECUTING QUESTION 3\n");
    FeeStatusList *list_Of_Eligible_Students = Question3(AttendanceListArray, head_Of_ASL, head_Of_FSL);
    printFSL(list_Of_Eligible_Students);
    //

    printf("\nEXECUTING QUESTION 4\n");
    AttendanceList *list_Of_Low_Attendance = Question4(AttendanceListArray);
    printARL(list_Of_Low_Attendance);
    //

    //

    printf("\nEXECUTING QUESTION 5\n");
    Question5(head_Of_FSL, AttendanceListArray);
    //

    printf("\nEXECUTING QUESTION 6\n");
    FeeStatusList *list_Of_Defaulters = Question6(list_Of_Low_Attendance, head_Of_FSL);
    printf("\n");
    printFSL(list_Of_Defaulters);
    printf("\n");
    //

    //

    //

    //

    //

    //

    //

    //
    printf("\nMCC\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf(" %d ", MCC[i][j]);
        }
        printf("\n");
    }
    printf("\nMCA\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf(" %d ", MCA[i][j]);
        }
        printf("\n");
    }

    //

    //

    //

    printf("\nDELETING 1st LIST\n");
    DeleteGSRL(head_Of_GSRL);
    printf("\nDELETING 2nd LIST(all)\n");
    DeleteAllARL(AttendanceListArray);
    printf("\nDELETING 3rd LIST\n");
    DeleteFSL(head_Of_FSL);
    printf("\nDELETING 4th LIST\n");
    DeleteASL(head_Of_ASL);
    printf("\nDELETING  LIST of low attendance\n");
    DeleteARL(list_Of_Low_Attendance);
    printf("\nDELETING  LIST of Eligible Students\n");
    DeleteFSL(list_Of_Eligible_Students);
    printf("\nDELETING  LIST of Defaulters\n");
    DeleteFSL(list_Of_Defaulters);
    return 0;
}

void Question1(GR_StudentList *head)
{
    printGSRL(head);
}

void Question2(ApplicantStatusList *head)
{
    ApplicantStatusList *ptr = head;
    while (ptr)
    {
        if (ptr->apply == NOTAPPLIED)
        {
            printf("\t %20s", ptr->name);
            printf("\t %s", ptr->roll);
            printf("\tAPPLICANT STATUS= %d\n", ptr->apply);
        }

        ptr = ptr->next;
    }
}

FeeStatusList *Question3(AttendanceList *AttendanceListArray[4], ApplicantStatusList *head_Of_ASL, FeeStatusList *head_Of_FSL)
{
    FeeStatusList *list_Of_Eligible_Students = NULL;
    for (int i = 0; i < 4; i++)
    {
        AttendanceList *ptr_ARL = AttendanceListArray[i];
        while (ptr_ARL)
        {
            if (ptr_ARL->attendance_Percent > 75) //1st criteria
            {
                FeeStatusList *ptr_FSL = head_Of_FSL;
                while (ptr_FSL)
                {
                    if (strcmp(ptr_ARL->rollno, ptr_FSL->roll) == 0 && ptr_FSL->fee == CLEAR) //2nd criteria
                    {
                        ApplicantStatusList *ptr_ASL = head_Of_ASL;
                        while (ptr_ASL)
                        {
                            if (strcmp(ptr_ASL->roll, ptr_FSL->roll) == 0 && ptr_ASL->apply == APPLIED) //3rd criteria
                            {
                                list_Of_Eligible_Students = InsertAtStartFSL(ptr_FSL->fee, ptr_FSL->roll, list_Of_Eligible_Students);
                            }
                            ptr_ASL = ptr_ASL->next;
                        }
                    }
                    ptr_FSL = ptr_FSL->next;
                }
            }
            ptr_ARL = ptr_ARL->next;
        }
    }
    list_Of_Eligible_Students = MergeSortFSL(list_Of_Eligible_Students);
    return list_Of_Eligible_Students;
}

AttendanceList *Question4(AttendanceList *AttendanceListArray[4])
{
    AttendanceList *list_Of_Low_Attendance = NULL;
    AttendanceList *ptr_ARL;
    for (int i = 0; i < 4; i++)
    {
        ptr_ARL = AttendanceListArray[i];
        while (ptr_ARL)
        {
            if (ptr_ARL->attendance_Percent <= 75)
            {
                list_Of_Low_Attendance = InsertAtStartARL(ptr_ARL->rollno, ptr_ARL->dep, ptr_ARL->attendance_Percent, list_Of_Low_Attendance);
            }
            ptr_ARL = ptr_ARL->next;
        }
    }
    list_Of_Low_Attendance = MergeSortARL(list_Of_Low_Attendance);
    return list_Of_Low_Attendance;
}

void Question5(FeeStatusList *head_Of_FSL, AttendanceList *AttendanceListArray[4])
{
    FeeStatusList *ptr_FSL = head_Of_FSL;
    while (ptr_FSL)
    {
        if (ptr_FSL->fee == PENDING)
        {
            for (int i = 0; i < 4; i++)
            {
                AttendanceList *ptr_ARL = AttendanceListArray[i];
                while (ptr_ARL)
                {
                    if (strcmp(ptr_FSL->roll, ptr_ARL->rollno) == 0 && ptr_ARL->attendance_Percent > 75)
                    {
                        printf("\tRoll Number=%s", ptr_FSL->roll);
                        printf("\tFee Status=%d", ptr_FSL->fee);
                        printf("\tAttedance%%=%f\n", ptr_ARL->attendance_Percent);
                    }
                    ptr_ARL = ptr_ARL->next;
                }
            }
        }
        ptr_FSL = ptr_FSL->next;
    }
}

int findDepIndex(char *dep)
{
    int c = dep[0], x = -1;
    switch (c)
    {
    case 'C': //CS or CV
        if (dep[1] == 'S')
            x = 0;
        else
            x = 1;
        break;
    case 'E':
        if (dep[1] == 'C')
            x = 2;
        else
            x = 3;
        break;
    case 'M':
        x = 4;
        break;
    default:
        printf("\nERROR !!! DEPARTMENT NOT FOUND\n");
        break;
    }
    return x;
}
void print_Dep_With_Most_Defaulters(int count_Of_Defaulters_Department[5])
{
    int maxcount = count_Of_Defaulters_Department[0];
    int idx = 0;
    for (int i = 1; i < 5; i++)
    {
        if (maxcount < count_Of_Defaulters_Department[i])
        {
            maxcount = count_Of_Defaulters_Department[i];
            idx = i;
        }
    }
    printf("\nMost Defaulters From :->");
    switch (idx)
    {
    case 0:
        printf("\tCS");
        break;
    case 1:
        printf("\tCV");
        break;
    case 2:
        printf("\tEC");
        break;
    case 3:
        printf("\tEE");
        break;
    case 4:
        printf("\tME");
        break;

    default:
        printf("\t Error dep doesnt exist");
        break;
    }
}
FeeStatusList *Question6(AttendanceList *list_Of_Low_Attendance, FeeStatusList *head_Of_FSL)
{
    FeeStatusList *list_Of_Defaulters = NULL;
    AttendanceList *ptr_ARL = list_Of_Low_Attendance;
    int count_Of_Defaulters_Department[5] = {0};
    while (ptr_ARL)
    {
        int dep_Index = findDepIndex(ptr_ARL->dep);
        count_Of_Defaulters_Department[dep_Index] += 1;

        list_Of_Defaulters = InsertAtStartFSL(0, ptr_ARL->rollno, list_Of_Defaulters);

        ptr_ARL = ptr_ARL->next;
    }
    // Now fee status list
    list_Of_Defaulters = MergeSortFSL(list_Of_Defaulters);

    FeeStatusList *ptr_FSL = head_Of_FSL;
    while (ptr_FSL)
    {
        if (ptr_FSL->fee == PENDING)
        {
            //now check if already in list or not
            FeeStatusList *temp = list_Of_Defaulters;
            int found = 0;
            while (temp && found == 0)
            {
                if (strcmp(temp->roll, ptr_FSL->roll) == 0)
                {
                    found = 1; //already in list
                }
                temp = temp->next;
            }
            if (found == 0)
            {
                list_Of_Defaulters = InsertAtStartFSL(ptr_FSL->fee, ptr_FSL->roll, list_Of_Defaulters);
                char dep[3];
                dep[0] = ptr_FSL->roll[4];
                dep[1] = ptr_FSL->roll[5];
                dep[2] = '\0';
                int dep_Index = findDepIndex(dep);
                count_Of_Defaulters_Department[dep_Index] += 1;
            }
        }

        ptr_FSL = ptr_FSL->next;
    }

    print_Dep_With_Most_Defaulters(count_Of_Defaulters_Department);

    list_Of_Defaulters = MergeSortFSL(list_Of_Defaulters);
    return list_Of_Defaulters;
}
