# Device Project Generator

## 목적

- 리눅스의 장치 드라이버 모듈의 템플릿을 미리 작성해 놓고
- 플레이스 홀더를 치환하는 방식으로 빠르게 소스 코드의 기틀을 마련하여
- 상용구코드 입력 작업을 줄이며,
- Makefile을 제공하여 빌드 및 테스트 커맨드를 추상화함으로써
- 장치 드라이버 개발 과정의 효율화를 추구한다.



## 캐릭터 디바이스 생성

Character Device Driver를 위한 파일들을 생성합니다

```bash
$ make cdev [name={식별자}] [dest={프로젝트 생성 위치}] [count={디바이스 수}] [license={라이센스}]
```

### **패러미터**

- `name={식별자}`

  `name`값은 파일 전반적인 식별자 역할을 하는 중요한 변수입니다. 소스 파일명 / 프로젝트 폴더명 / 소스코드 내 함수들의 접두사 / 디바이스 파일명 등으로 활용됩니다.

  ```bash
  $ make cdev name=my_keyboard
  ```

  이 예제로부터 설정되는 이름들의 리스트입니다.

  - 프로젝트 폴더명 `./my_keyboard_project` 
  - 소스 파일명 `cdev_my_keyboard.c` 
  - 헤더 파일명 `cdev_my_keyboard.h` 
  - 디바이스명 `#define DEV_NAME "my_keyboard"` 
  - `my_keyboard_open()`
  - `my_keyboard_release()`
  - `my_keyboard_exit()`
  - ... 기타 모듈 함수명
  - `/dev/my_keyboard` 디바이스 파일명

  

- `dest={경로}`

  `dest`는 프로젝트 폴더가 자리할 디렉토리를 지정합니다. 기본값은 현재 디렉토리 `.`입니다. 

  ```bash
  $ make cdev name=my_censor dest=..
  ```

  이 예제를 실행하면 `../my_censor_project` 디렉토리가 생성됩니다. 

  

- `count={디바이스 수}`

  `count`는 소스 코드에서 디바이스의 개수를 요구하는 매개변수에 치환됩니다. 기본값은 `1`입니다. `count `값이 삽입되는 함수는 3 종류입니다.

  - `cdev_add(cdev*, dev_t, {dev_count})`
  - `register_chrdev_region(dev_t*, {dev_count}, {dev_name})`
  - `unregister_chrdev_region(dev_t, {dev_count})`

  

- `license={라이센스}`

  소스코드 `MODULE_LICENSE({license})` 에 삽입됩니다. 따옴표를 붙여서 삽입해주므로 커맨드에서 따옴표를 감싸지 않습니다. 기본값은 `GPL`입니다.



## 캐릭터 모듈 빌드

**프로젝트 폴더에서**

```bash
$ make
```

프로젝트 폴더에도 Makefile이 존재합니다. Makefile의 템플릿의 식별자가 치환되어 생성된 것입니다. `$ make`를 실행하면 모듈을 빌드하고 `.ko` 파일이 생성될 것입니다.



## 디바이스 파일 생성

**프로젝트 폴더에서**

```bash
$ make node
```

프로젝트 폴더에서`$ make node`를 실행하면 장치 파일 `/dev/{dev-name}`가 생성됩니다. 



## 간편한 테스트 코드 실행

**프로젝트 폴더에서**

```bash
$ make run={c소스파일}
```

지정한 소스파일을 컴파일하여 실행합니다



## 사용 시나리오

장치 드라이버 소스코드 생성기의 용법을 익혔으니 실제 드라이버 모듈 개발 순서에 맞추어 이용해 봅시다.

```bash
# DeviceProjectGenerator 폴더에서
$ make cdev name=monitor dest=..
$ cd ../monitor_project

# 모듈 코드 수정
$ vim cdev_monitor.h
$ vim cdev_monitor.c

# 모듈 빌드
$ make

# 모듈을 커널에 삽입
$ sudo insmod cdev_monitor.ko

# 장치의 메이저 넘버를 확인하여 장치파일 생성
$ make node

# 임의의 코드 작성과 실행
$ vim my_test.c
$ make run=test.c

# 정돈
$ make clean
$ sudo rmmod cdev_monitor
$ sudo rm /dev/monitor

```



## 템플릿
### templates/cdev

   캐릭터 디바이스용 템플릿 보관소

  `cdev.c` : 흔한 캐릭터 디바이스 모듈입니다. 자유롭게 수정하여 모듈을 작성하십시오.

  `cdev.h` : ioctl cmd를 설정하는 용도로 쓰이고 있습니다. 원하는 목적으로 사용하시기 바랍니다.

  `cdev_make` : 프로젝트 Makefile의 원형입니다. 

### 플레이스 홀더

`{dev_name}` <- name

`{license}` <- license

`{dev_vim count}` <- count

`{object_file}` <- cdev_`{dev_name}`.o



## Generator의 Makefile

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



파일을 들여다 보면`sed` 명령어를 사용하여 플레이스 홀더를 치환합니다. 단순한 파일이므로 원하는 기능이 있다면 수정하여 사용하시기 바랍니다.
