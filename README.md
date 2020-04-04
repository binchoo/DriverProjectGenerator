# Linux Driver Template

## 목적

- 리눅스 장치 드라이버 모듈의 소스코드를 템플릿화
- 템플릿의 플레이스 홀더를 치환하는 방법으로 소스코드의 바탕을 마련
- 소스코드의 상용구 코드 입력 작업 회피
- 빌드 및 테스트 커맨드를 추상화한 Makefile을 제공
- 장치 드라이버 개발 과정의 효율화
- ...를 하기 위해서 임베디드시스템소프트웨어 수업을 잘 듣고 개선시켜 나가겠습니다
---

## 기능 소개

### 프로젝트 생성하기

프로젝트 폴더를 생성하고 그 안에는 모듈 개발에 필요한 .c, .h, Makefile 파일을 제공합니다.

#### 캐릭터 디바이스 프로젝트 생성

```bash
$ make cdev [name={식별자}] [dest={프로젝트 생성 위치}] [count={디바이스 수}] [license={라이센스}]
```

#### 파라미터 name

`name={식별자}`

파일 **전반적인 식별자 역할**을 하는 중요한 변수입니다. 프로젝트에서 생성되는 요소들의 이름은 `name`에 의존합니다. 소스 파일명 / 프로젝트 폴더명 / 소스코드 내 함수들의 접두사 / 디바이스 파일명 등에 활용됩니다. 

```bash
$ make cdev name=my_keyboard
```

`name=my_keyboard`로 했을 경우 설정되는 이름들을 리스트로 나타냈습니다

- 프로젝트 폴더명 

  `./my_keyboard_project` 

- 소스 파일명

   `cdev_my_keyboard.c` 

- 헤더 파일명 

  `cdev_my_keyboard.h` 

- 리눅스 모듈 명

  `cdev_my_keyboard.ko`

- 디바이스 명

  `#define DEV_NAME "my_keyboard"` 

- 기본적인 드라이버 모듈 함수 7가지

  `my_keyboard_open()`

  `my_keyboard_release()`

  `my_keyboard_read()`

  `my_keyboard_write()`

  `my_keyboard_ioctl()`

  `my_keyboard_init()`

  `my_keyboard_exit()`

- 디바이스 파일명

  `/dev/my_keyboard` 

#### 파라미터 dest

`dest={경로}`

 프로젝트 폴더가 자리할 디렉토리를 지정합니다. 기본값은 현재 디렉토리 `.`입니다. 

```bash
$ make cdev name=my_censor dest=..
```

이 예제를 실행하면 프로젝트 폴더  `../my_censor_project` 가 생성됩니다. 

#### 파라미터 count

`count={디바이스 수}`

소스 코드 템플릿에서 디바이스의 개수를 요구하는 부분에 치환됩니다. 기본값은 `1`입니다. 

`count` 값이 삽입되는 함수는 3 종류입니다.



- `cdev_add(cdev*, dev_t, {dev_count})`
- `register_chrdev_region(dev_t*, {dev_count}, {dev_name})`
- `unregister_chrdev_region(dev_t, {dev_count})`

#### 파라미터 license

- `license={라이센스}`

  소스 코드 템플릿의 앞 부분에는 라이선스를 명시하는  `MODULE_LICENSE({license});`  를 발견할 수 있습니다. 이 부분에 사용자가 지정한 라이센스를 치환합니다. 기본값은 `GPL`입니다. 따옴표를 붙여서 삽입해주므로 값을 따옴표로 감싸서 입력하지 않습니다.

### 모듈 빌드하기

**생성한 프로젝트 폴더에서**

```bash
$ make
```

프로젝트 폴더에도 Makefile이 존재하고 있습니다. 이 Makefile은 Makefile 템플릿에서 유래된 것입니다.

프로젝트 폴더에서 `$ make`를 입력하면 모듈을 빌드되어 `.ko` 파일이 생성됩니다.



### 디바이스 파일 만들기

**생성한 프로젝트 폴더에서**

```bash
$ make node
```

프로젝트 폴더에서 `$ make node`를 실행하면 장치 파일 `/dev/{dev-name}`가 생성됩니다. 



### 간편한 테스트 코드 실행

**생성한 프로젝트 폴더에서**

```bash
$ make run={c소스파일}
```

지정한 소스파일을 컴파일하여 실행하는 단순한 기능입니다. 인자를 넘겨주는 기능은 구현되지 않았습니다.



## <span style="color:#ff5c40">사용 시나리오</span>

드라이버 소스 코드 생성기의 용법을 익혔으니 

실제 드라이버 모듈 개발 순서에 맞추어 이용해 봅시다.

### 1단계. 프로젝트 생성

이번에 만들고 싶은 장치 드라이버는 모니터와 관련되어 있습니다. 그래서 모듈의 이름을 `monitor`로 할 것입니다.

```bash
# DeviceProjectGenerator 폴더에서
$ make cdev name=monitor dest=..
$ cd ../monitor_project
```

### 2단계. 모듈 코드 수정과 빌드

만들어진 프로젝트 폴더 안에는 소스코드와 헤더파일이 존재합니다. 이 파일들을 개발자가 원하는 대로 수정합시다. 소스코드 작성이 완료되었다면 빌드합시다.

```bash

# 모듈 코드 수정
$ vim cdev_monitor.h
$ vim cdev_monitor.c
$ make
```

### 3단계. 모듈 삽입

빌드가 완료되었다면 `.ko` 파일이 생성되었을 겁니다. `$ insmod` 명령어를 사용하여 커널에 모듈을 삽입합시다.

```bash
# 모듈을 커널에 삽입
$ sudo insmod cdev_monitor.ko
```

### 4단계. 장치 파일 만들기

응용 프로그램이 장치를 사용할 때는 `장치 파일`을 쓰게 되어있습니다.

모듈을 커널에 삽입하였다면 모듈에 메이저 넘버가 할당되어 있을 것입니다.  장치 파일은 대응되는 장치 드라이버의 메이저 넘버를 알고있어서 올바른 드라이버에 기능을 요청할 수 있습니다. 

그러므로 장치의 메이저 넘버를 알아내서  `mknod` 명령어를 통해 장치 파일을 만들어 주어야 합니다. 

이 과정은 추상화된 `$ make node` 명령어로 실현할 수 있습니다. 마이너 넘버를 고려하지 않으므로 필요하다면 `Makefile`을 바꾸시기 바랍니다.

```bash
# 장치의 메이저 넘버를 확인하여 장치파일 생성
$ make node
```

### 5단계. 테스트 응용프로그램 실행

생성한 장치 파일을 통해 드라이버 동작을 테스트하는 응용 프로그램을 작성할 수 있게 되었습니다. gcc로 컴파일하여 실행하면 되지만, `$make test run={c소스코드}` 명령이 이 과정을 축약합니다.

```bash
# 테스트 응용 빌드와 실행
$ vim app.c
$ make test run=app.c
```

### 6단계. 정리

장치 드라이버가 필요하지 않게 되어 커널에서 제거하고, 장치 파일도 없애고자 한다면 다음과 같은 명령어로 실현할 수 있습니다. 

`$ make clean`은 프로젝트 폴더의 산출물들을 없애줍니다.

```bash
# 정리하기
$ sudo rmmod cdev_monitor
$ rm /dev/monitor
$ make clean
```



## 템플릿 파일

### templates/cdev/

   캐릭터 디바이스용 템플릿 보관소입니다.

  `cdev.c` : 캐릭터 디바이스 모듈입니다. 자유롭게 수정하십시오.

  `cdev.h` : ioctl cmd를 설정하는 용도로 쓰이고 있습니다. 자유롭게 수정하여 원하는 목적에 맞게 사용하십시오.

  `cdev_make` : 프로젝트에서 사용하는 Makefile의 원형입니다. 

### 플레이스 홀더

`{dev_name}` <- name

`{license}` <- license

`{dev_count}` <- count

`{object_file}` <- cdev_`{dev_name}`.o



## 치환 원리

`sed` 명령어를 사용하여 플레이스 홀더를 치환합니다. 

Makefile은 단순한 구조를 갖고 있으므로 원하는 기능이 있다면 수정하여 사용하시기 바랍니다.

```Makefile
...
cdev: makedir cdevc cdevh cdevmk

makedir:
        mkdir $(PROJECT_DIR)

cdevc:
        cp $(CDEV_C_TEMPLATE) $(CDEV_C)
        sed -i 's/{dev_name}/$(name)/g' $(CDEV_C)
        sed -i 's/{dev_count}/$(count)/g' $(CDEV_C)
        sed -i 's/{license}/\"$(license)\"/g' $(CDEV_C)
        sed -i 's/{header}/$(FILE_NAME).h/g' $(CDEV_C)

...
```
