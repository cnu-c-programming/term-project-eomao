# Mini Student Shell 보고서

## 1. 프로젝트 개요

이 프로젝트는 C 언어로 학생 정보를 관리하는 간단한 shell 프로그램을 구현한 것이다. 학생 데이터는 CSV 파일에서 읽어 오고, 프로그램 내부에서는 linked list로 관리한다. 사용자는 명령어를 입력해서 학생 목록 조회, 검색, 추가, 삭제, 수정, 저장, 다시 불러오기, 통계 출력 등을 실행할 수 있다.

프로그램은 Admin Program과 Client Program 두 가지로 나누어 빌드된다. 두 실행 파일은 같은 소스 코드를 사용하지만, Makefile에서 `-DADMIN_MODE`, `-DCLIENT_MODE` 전처리 플래그를 다르게 주어 사용할 수 있는 명령어를 구분한다.

## 2. 빌드 및 실행 방법

```bash
make admin
make client
make all
make clean
```

실행 형식은 다음과 같다.

```bash
./admin_shell students.csv
./client_shell students.csv
./admin_shell students.csv -f commands.txt
./client_shell students.csv -f commands.txt
```

CSV 파일 인자가 없으면 `Usage` 메시지를 출력하고 종료한다. `-f` 뒤에 명령어 파일명이 없을 때도 에러 메시지를 출력한다.

## 3. 파일 구조

```text
main.c       프로그램 시작, 인자 처리, shell 루프, 명령어 파일 실행
student.h    Student 구조체와 linked list 함수 선언
student.c    linked list 삽입, 삭제, 검색, 수정, 정렬, 메모리 해제
file_io.h    CSV load/save 함수 선언
file_io.c    CSV 파일 읽기, 헤더 검사, CSV 저장
command.h    명령 실행 결과 enum과 명령 처리 함수 선언
command.c    명령어 파싱과 각 명령어 처리
Makefile     Admin/Client 빌드 자동화
students.csv 100명 샘플 CSV 파일
commands.txt 샘플 명령어 파일
```

## 4. 단계별 구현 과정

### 1단계: 기본 파일 구조 작성

처음에는 과제에서 요구한 모듈 구조에 맞춰 파일을 나누었다. `main.c` 하나에 모든 기능을 넣지 않고, 학생 자료구조는 `student.h`, `student.c`, CSV 파일 입출력은 `file_io.h`, `file_io.c`, 명령어 처리는 `command.h`, `command.c`로 분리했다. `Makefile`의 `SRCS`에는 `main.c student.c file_io.c command.c`를 넣어서 Admin/Client 빌드에서 같은 소스 파일을 사용하도록 했다.

이 단계에서 `Student` 구조체와 `StudentList` 구조체를 먼저 선언했다. 학생 한 명은 `id`, `name`, `score`, `next`를 가지며, 전체 목록은 `StudentList`의 `head` 포인터로 시작하게 설계했다. 아직 기능을 전부 구현하지 않고, 각 파일이 어떤 역할을 맡을지만 먼저 정했다.

### 2단계: 프로그램 실행 흐름 구현

두 번째 단계에서는 `main.c`에서 프로그램 시작 흐름을 만들었다. `main()`에서 CSV 파일 경로와 `-f` 명령어 파일 옵션을 파싱하도록 했고, 인자가 없거나 잘못 들어오면 `Usage`를 출력하도록 했다.

`run_shell()`은 일반 대화형 shell 실행을 담당한다. CSV 파일을 먼저 로드한 뒤 `[Admin Program]` 또는 `[Client Program]` 배너를 출력하고, `admin>` 또는 `client>` 프롬프트를 반복해서 보여 준다. 사용자가 입력한 한 줄은 `execute_command()`로 넘긴다. `run_command_file()`은 명령어 파일을 열어 한 줄씩 읽고 실행하는 함수로 만들었다.

### 3단계: CSV 파일 열기와 헤더 검사

세 번째 단계에서는 `file_io.c`의 `load_students()`에서 CSV 파일을 실제로 열고 첫 줄 헤더를 검사하도록 했다. 올바른 헤더는 `id,name,score` 하나로 정했다. 파일을 열 수 없거나 헤더가 없거나 헤더 문자열이 다르면 `Error` 메시지를 출력하고 실패값을 반환한다.

이 단계에서는 아직 학생 데이터를 linked list에 넣지 않고, 파일을 읽을 수 있는지와 CSV 형식이 맞는지만 확인했다. `main.c`에서는 `load_students()`가 실패했을 때 `Loaded -1 students` 같은 잘못된 출력이 나오지 않도록 실행을 중단하게 처리했다.

### 4단계: CSV 데이터 로드와 linked list 저장

네 번째 단계에서는 CSV의 데이터 줄을 파싱해서 linked list에 저장하도록 했다. `file_io.c`의 `parse_student_line()`은 `id,name,score` 형식의 한 줄을 `id`, `name`, `score` 값으로 나눈다. 이후 `add_student()`를 호출해서 `StudentList`에 추가한다.

`student.c`에서는 `add_student()`를 구현했다. ID는 양수인지, 점수는 0부터 100 사이인지, 이름이 비어 있지 않은지 검사한다. 또 `find_student()`로 중복 ID가 있는지 확인한다. 새 노드는 `malloc()`으로 만들고, 리스트가 비어 있으면 head에 넣고, 이미 노드가 있으면 마지막 노드 뒤에 연결한다.

### 5단계: list와 find 조회 명령 구현

다섯 번째 단계에서는 조회 기능인 `list`와 `find`를 구현했다. `command.c`의 `handle_list()`는 linked list를 처음부터 끝까지 순회하면서 `ID`, `Name`, `Score`를 출력한다. 학생이 한 명도 없으면 `No students found.`를 출력한다.

`handle_find()`는 입력받은 ID를 검사한 뒤 `find_student()`를 호출한다. 학생을 찾으면 `ID:`, `Name:`, `Score:` 형식으로 한 줄씩 출력한다. ID가 숫자가 아니거나 0 이하이면 `Error`, 해당 ID 학생이 없으면 `Error: student not found.`를 출력하도록 했다. 이 두 명령은 Admin과 Client 모두 사용할 수 있다.

### 6단계: add 명령 구현

여섯 번째 단계에서는 Admin 전용 명령인 `add <id> <name> <score>`를 구현했다. `command.c`에서 `#ifdef ADMIN_MODE` 안에 `handle_add()`를 두어 Client 빌드에는 포함되지 않게 했다.

`handle_add()`는 인자 개수, ID, 점수, 이름, 중복 ID를 순서대로 검사한다. 점수는 `parse_score()`로 0 이상 100 이하인지 확인한다. 검사가 끝나면 `student.c`의 `add_student()`를 호출해서 실제 linked list에 노드를 추가한다. 성공하면 `Student added.`를 출력한다. Client Program에서 `add`를 입력하면 명령어 목록에 없으므로 `Unknown command or permission denied.`가 출력된다.

### 7단계: delete와 update 명령 구현

일곱 번째 단계에서는 Admin 전용 `delete`와 `update`를 구현했다. `student.c`의 `delete_student()`는 현재 노드와 이전 노드를 같이 추적하면서 삭제할 ID를 찾는다. 삭제 대상이 head이면 `list->head`를 다음 노드로 바꾸고, 중간 노드이면 이전 노드의 `next`를 삭제 대상의 다음 노드로 연결한다. 삭제한 노드는 `free()`로 해제한다.

`update_student()`는 ID로 학생을 찾은 뒤 점수만 바꾼다. 점수가 범위를 벗어나거나 학생을 찾지 못하면 실패를 반환한다. `command.c`의 `handle_delete()`, `handle_update()`는 이 함수들을 호출하고, 성공하면 `Student deleted.` 또는 `Student updated.`를 출력한다.

### 8단계: save와 reload 구현

여덟 번째 단계에서는 CSV 저장과 다시 불러오기를 구현했다. `file_io.c`의 `save_students()`는 파일을 쓰기 모드로 열고, 첫 줄에 `id,name,score` 헤더를 쓴 다음 linked list를 순회하면서 각 학생을 CSV 형식으로 저장한다.

`command.c`의 `handle_save()`는 Admin 전용으로 두었다. 저장에 성공하면 `Saved N students to <file>.` 형식으로 출력한다. `reload`는 Admin과 Client 모두 사용할 수 있게 했다. `handle_reload()`는 현재 linked list를 `clear_student_list()`로 비운 뒤 `load_students()`로 같은 CSV 파일을 다시 읽는다. 그래서 저장하지 않은 변경사항은 `reload` 후 사라진다.

### 9단계: stats, help, clear 구현

아홉 번째 단계에서는 보조 명령어를 추가했다. `handle_stats()`는 linked list를 순회하면서 학생 수, 점수 합계, 최고점, 최저점을 계산한다. 평균은 `(double)sum / count`로 계산하고 `%.1f`로 출력해서 `Average: 90.0`처럼 소수점 한 자리가 나오게 했다. 학생이 없으면 `No student data available.`를 출력한다.

`handle_help()`는 사용할 수 있는 명령어 목록을 출력한다. Admin 빌드에서는 `save`, `add`, `delete`, `update`도 출력하고, Client 빌드에서는 이 수정 명령어들을 출력하지 않도록 `#ifdef ADMIN_MODE`로 구분했다. `handle_clear()`는 ANSI escape sequence인 `\033[2J\033[H`를 출력해서 화면을 지우도록 했다.

### 10단계: 명령어 파일 처리와 마무리 보완

열 번째 단계에서는 `-f` 명령어 파일 처리를 보완했다. `main.c`에 `trim_spaces()`를 추가해서 명령어 앞뒤 공백을 제거했다. 그래서 `  list  `처럼 공백이 있는 줄도 정상적으로 `list`로 처리된다. 빈 줄과 `#`로 시작하는 주석 줄은 실행하지 않는다.

명령어 파일 실행 시에는 실제 실행되는 명령만 번호를 세도록 `command_no`를 사용했다. 따라서 주석과 빈 줄은 `[command file:N]` 번호에 포함되지 않는다. 명령어 실행 결과가 `COMMAND_ERROR`이면 `Skipped line N.`을 출력하고 다음 명령어를 계속 실행한다. `exit`가 나오면 반복을 중단해서 뒤에 있는 명령어는 실행하지 않는다.

마지막 검수 과정에서 보너스 기능인 `sort name`, `sort score`도 추가했다. `student.c`의 `sort_students_by_name()`, `sort_students_by_score()`는 인접한 학생 데이터를 비교해서 필요한 경우 값을 교환한다. `command.c`의 `handle_sort()`는 정렬 기준이 `name` 또는 `score`인지 확인하고, 잘못된 기준이면 `Error: invalid sort key.`를 출력한다. 또한 100명 샘플 데이터인 `students.csv`와 샘플 명령어 파일인 `commands.txt`를 추가했다.

## 5. 자료구조 설계

학생 한 명은 `Student` 구조체로 저장한다.

```c
typedef struct Student {
    int id;
    char name[32];
    int score;
    struct Student *next;
} Student;
```

전체 학생 목록은 `StudentList` 구조체의 `head` 포인터로 관리한다. 새 학생은 리스트의 마지막에 추가한다. 삭제할 때는 이전 노드 포인터를 함께 추적해서 head 삭제와 중간 노드 삭제를 모두 처리한다. 프로그램 종료 전에는 `clear_student_list()`로 동적 할당된 노드를 모두 해제한다.

정렬 기능은 linked list 노드의 연결 자체를 바꾸지 않고, 인접 노드의 `id`, `name`, `score` 값을 교환하는 방식으로 구현했다. `sort name`은 이름 오름차순, `sort score`는 점수 오름차순으로 정렬한다.

## 6. CSV 파일 입출력

CSV 파일 형식은 다음과 같다.

```csv
id,name,score
1,Alice,90
2,Bob,85
3,Charlie,95
```

`load_students()`는 파일을 열고 첫 줄이 정확히 `id,name,score`인지 검사한다. 헤더가 다르거나 데이터 형식이 잘못되면 `Error` 메시지를 출력하고 실패를 반환한다.

각 데이터 줄은 `id,name,score` 형식으로 파싱한다. `id`는 양수, `score`는 0 이상 100 이하만 허용한다. 중복 ID나 잘못된 점수도 잘못된 CSV 데이터로 처리한다.

`save_students()`는 현재 linked list 내용을 같은 CSV 형식으로 저장한다. 항상 첫 줄에 `id,name,score` 헤더를 먼저 쓰고, 이후 학생 데이터를 리스트 순서대로 저장한다.

## 7. 명령어 구현

Admin Program에서 사용할 수 있는 명령어는 다음과 같다.

```text
list
find <id>
stats
help
clear
reload
sort name
sort score
save
add <id> <name> <score>
delete <id>
update <id> <score>
exit
```

Client Program에서는 조회 중심 명령어만 사용할 수 있다.

```text
list
find <id>
stats
help
clear
reload
sort name
sort score
exit
```

Client Program에서 `save`, `add`, `delete`, `update`를 입력하면 `Unknown command or permission denied.`를 출력한다.

## 8. 명령어 파일 처리

`-f commands.txt` 옵션을 사용하면 명령어 파일을 위에서 아래로 실행한다. 실행되는 명령은 다음 형식으로 출력한다.

```text
[command file:1] list
```

빈 줄과 `#`로 시작하는 주석 줄은 무시한다. 줄 앞뒤 공백도 제거한 뒤 처리한다. 명령어 실행 중 오류가 발생하면 `Skipped line N.`을 출력하고 다음 명령어를 계속 실행한다. `exit` 명령을 만나면 이후 명령어는 실행하지 않는다.

## 9. 예외 처리

다음 상황에서 에러 메시지를 출력하도록 처리했다.

- CSV 파일을 열 수 없는 경우
- CSV 헤더가 잘못된 경우
- CSV 데이터 형식이 잘못된 경우
- 없는 학생을 검색, 삭제, 수정하는 경우
- 중복 ID로 학생을 추가하는 경우
- ID가 양수가 아닌 경우
- 점수가 0~100 범위를 벗어난 경우
- 명령어 인자가 부족하거나 잘못된 경우
- 알 수 없는 명령어를 입력한 경우
- Client Program에서 수정 명령어를 입력한 경우

## 10. 테스트 결과

다음 명령으로 테스트했다.

```bash
gcc -Wall -Wextra -std=c11 -DADMIN_MODE main.c student.c file_io.c command.c -o /tmp/admin_shell_check
gcc -Wall -Wextra -std=c11 -DCLIENT_MODE main.c student.c file_io.c command.c -o /tmp/client_shell_check
python3 grader.py /tmp/admin_shell_check /tmp/client_shell_check students.csv
```

결과는 다음과 같다.

```text
필수 점수: 108 / 100 pt
보너스 점수: 15 / 15 pt
어드밴스드: 30 / 30 pt
총 점수: 153 / 145 pt
```

빌드는 Admin/Client 모두 경고 없이 통과했다.
