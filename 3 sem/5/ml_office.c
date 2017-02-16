#include "sthead.h"
#include <pthread.h>

#define INPUT_LEN 6
#define NUM_TASKS 2
#define CLERK_TASKNO 0
#define SCAN_TASKNO 1

#define CLERKS_NUM_ARGNO 0
#define DOCS_NUM_ARGNO 1
#define DOC_TIME_ARGNO 2
#define QUEUE_LEN_ARGNO 3
#define SCAN_NUM_ARGNO 4
#define SCAN_TIME_ARGNO 5

int queue_count = 0;
int max_queue_len = 0;
short* queue = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_free_places = PTHREAD_COND_INITIALIZER;

typedef struct clerk_task_t clerk_task_t;
struct clerk_task_t
{
	int id;
	int day_task;
	int doc_prep_time;
};

typedef int scan_task_t;

#undef __LOG

void usage(char* procname);
int run_office(char** in);
int prep_args(char** in, int* res);
int init_office(int* args, void** res);
void* scan_thread(void* task);
void* clerk_thread(void* task);

int main(int argc, char** argv)
{
	//проверка валидности входных данных
	if(argc != 7)
	{
		if(argc == 2 && !strcmp(argv[1], "--help"))
		{
			usage(argv[0]);
			return 0;
		}

		fprintf(stderr, "Invalid arguments:\n");
		usage(argv[0]);
		return BAD_INPUT;
	}
	
	return run_office(argv);
}

void usage(char* procname)
{
	fprintf(stdout, "Usage:\n"
		"\t" "%s " 
		"CLERKS_NUM DOCS_NUM DOC_TIME QUEUE_LEN SCAN_NUM SCAN_TIME\n"
		, procname);
}

int run_office(char** in)
{
	int i = 0, ret = 0, stat = 0;
	int args[INPUT_LEN];
	void* tasks[NUM_TASKS];
	void* threadret;
	tasks[CLERK_TASKNO] = NULL;
	tasks[SCAN_TASKNO] = NULL;

	//переводим аргументы коммандной строки в числа
	stat = prep_args(in, args);
	if(stat != 0)
	{
		usage(in[0]);
		fprintf(stderr, "All arguments must be positive decimals.\n");

		ret = FAILURE;
		goto run_office_ext;
	}

#ifdef __LOG
	printf("Arguments recieved:\n");
	for(i = 0; i < INPUT_LEN; i++)
	{
		printf("%d ", args[i]);
	}
	printf("\n");
#endif
	
	//инициализация переменных
	stat = init_office(args, tasks);
	if(stat != 0)
	{
		fprintf(stderr, "Can't initialize params: %s\n", strerror(errno));
		ret = FAILURE;
		goto run_office_ext;
	}

	//cоздаем потоки для сканеров, а потом для клерков
	pthread_t* threads = malloc(sizeof(threads[0]) * (args[CLERKS_NUM_ARGNO] + args[SCAN_NUM_ARGNO]));
	if(threads == NULL)
	{
		fprintf(stderr, "Can't allocate memory: %s\n", strerror(errno));
		ret = FAILURE;
		goto run_office_ext;
	}

	//задания для сканеров не изменяются
#ifdef __LOG
	printf("run_office:\tcreating %d scaners\n", args[SCAN_NUM_ARGNO]);
#endif
	for(i = 0; i < args[SCAN_NUM_ARGNO]; i++)
	{
		stat = pthread_create(&threads[i], 0, scan_thread, tasks[SCAN_TASKNO]);
		if(stat != 0)
		{
			fprintf(stderr, "Can't create thread: %s\n", strerror(errno));
			ret = FAILURE;
			goto run_office_ext;
		}
	}

	//задания для клерков отличаются только персональным номером клерка
#ifdef __LOG
	printf("run_office:\tcreating %d clerks\n", args[CLERKS_NUM_ARGNO]);
#endif
	for(i = 0; i < args[CLERKS_NUM_ARGNO]; i++)
	{
		stat = pthread_create(&threads[i + args[SCAN_NUM_ARGNO]], 0, 
			clerk_thread, (void*)((clerk_task_t*)tasks[CLERK_TASKNO] + i));
		if(stat != 0)
		{
			fprintf(stderr, "Can't create thread: %s\n", strerror(errno));
			ret = FAILURE;
			goto run_office_ext;
		}
	}

	//здесь только, если все потоки создались
	for(i = args[SCAN_NUM_ARGNO]; 
		i < (args[SCAN_NUM_ARGNO] + args[CLERKS_NUM_ARGNO]); i++)
	{
	#ifdef __LOG
		printf("run_office:\tjoining with clerk\n");
	#endif
		stat = pthread_join(threads[i], &threadret);
		if(stat != 0)
		{
			fprintf(stderr, "Can't join with clerk threads: %s\n", 
				strerror(stat));
			ret = FAILURE;
			goto run_office_ext;
		}
		else if(threadret != (void*)0)
		{
			fprintf(stderr, "One or more threads worked badly\n");
			ret = FAILURE;
			goto run_office_ext;
		}
	#ifdef __LOG
		printf("run_office:\tjoined with clerk\n");
	#endif
	}

run_office_ext:
	
	if(tasks[CLERK_TASKNO] != NULL)
	{
		free(tasks[CLERK_TASKNO]);
		free(tasks[SCAN_TASKNO]);
	}

	if(queue != NULL)
	{
		free(queue);
	}

	if(threads != NULL)
	{
		free(threads);
	}

	return ret;
}

//переводит строки, содержащие числа, в числа + проверяет, что кроме чисел
//информации в строках не содержится, а также нет чисел меньше нуля
int prep_args(char** in, int* res)
{
	char* check = NULL;

	for(int i = 0; i < INPUT_LEN; i++)
	{
		/*ошибкой является любая из следующих ситуаций:
			1) хотя бы одна из строк пуста (практически невозможно, так как
				строки тянутся из argv) errno = EINVAL
			2) хотя бы одна из строк cодержит информацию, кроме чисел
				(проверяем char **endptr, см. man strtol) -- 
				errno = EINVAL
			3) строк больше, чем INPUT_LEN (практически невозможно, так как
				строки тянутся из argv) -- errno = E2BIG 
		*/
		if(in[i + 1][0] == '\0')	//пустая строка
		{
			errno = EINVAL;
			return FAILURE;
		}

		res[i] = strtol(in[i + 1], &check, 0);
		if(errno != 0)			//errno = ERANGE, см. man strtol
		{
			return FAILURE;
		}
		else if(check[0] != '\0')	//строка содержит что-то, кроме цифр
		{
			fprintf(stderr, "Invalid argument: \"%s\"\n", in[i + 1]);
			errno = EINVAL;
			return FAILURE;
		}
	}

	if(in[INPUT_LEN + 1] != NULL)
	{
		errno = E2BIG;
		return FAILURE;
	}
}

//инициализация переменных
//res[CLERK_TASKNO] - "задание" для клерка
//res[SCAN_TASKNO] - "задание" для сканера
int init_office(int* args, void** res)
{
	//CLERKS_NUM DOCS_NUM DOC_TIME QUEUE_LEN SCAN_NUM SCAN_TIME
	clerk_task_t*  clerk = malloc(args[CLERKS_NUM_ARGNO] * sizeof(clerk[0]));
	if(clerk == NULL)
	{
		return FAILURE;
	}

	scan_task_t* scan = malloc(sizeof(scan[0]));
	if(scan == NULL)
	{
		free(clerk);
		return FAILURE;
	}

	queue = malloc(sizeof(queue[0]) * args[QUEUE_LEN_ARGNO]);
	if(queue == NULL)
	{
		return FAILURE;
	}

	max_queue_len = args[QUEUE_LEN_ARGNO];

	for(int i = 0; i < args[CLERKS_NUM_ARGNO]; i++)
	{
		clerk[i].id = i;
		clerk[i].day_task = args[DOCS_NUM_ARGNO];
		clerk[i].doc_prep_time = args[DOC_TIME_ARGNO];
	}

	//typedef int scan_task_t; - "задание" для сканера состоит только из
	//длительности сканирования
	*scan = args[SCAN_TIME_ARGNO];

	res[CLERK_TASKNO] = (void*)clerk;
	res[SCAN_TASKNO] = (void*)scan;

	return 0;
}

//функция, которую выполняют потоки-сканеры
//сканер ждет, пока в очереди появится хотя бы один документ и "сканирует" его
//сканирование заключается в том, что сканер ждет известное (лежащее в его задании)
//время, по истечении которого пишет на экран содержимое документа
void* scan_thread(void* task)
{
#ifdef __LOG
	printf("Scaner:\tis here, scan time = %d\n", *((scan_task_t*)task));
#endif
	void* ret = 0;

	while(true)
	{
#ifdef __LOG
		printf("Scaner:\tlocking the mutex\n");
#endif

		//ожидаем появления в очереди документов
		int stat = pthread_mutex_lock(&mutex);
		if(stat != 0)
		{
			fprintf(stderr, "Can't lock the mutex: %s\n", strerror(stat));
			ret = (void*)FAILURE;
			goto scan_thread_ext;
		}

#ifdef __LOG
		printf("Scaner:\tmutex locked\n");
#endif

		while(queue_count == 0)
		{
		#ifdef __LOG
			printf("Scaner:\twaiting for documents in queue\n");
		#endif

			stat = pthread_cond_wait(&queue_not_empty, &mutex);
			if(stat != 0)
			{
				fprintf(stderr, "Can't start condition wait: %s\n",
					strerror(stat));
				ret = (void*)FAILURE;
				goto scan_thread_ext;
			}
		}

		//mutex вновь "закрыт"
		if(queue_count == max_queue_len)
		{
		#ifdef __LOG
			printf("Scaner:\tbroadcasting the queue free places as it was full\n");
		#endif
			stat = pthread_cond_broadcast(&queue_free_places);
			if(stat != 0)
			{
				fprintf(stderr, "Can't broadcast condition: %s\n", 
					strerror(stat));
				ret = (void*)FAILURE;
				goto scan_thread_ext;
			}
		}

		//берем одно из заданий в очереди (будем брать из конца очереди, чтобы не было
		//необхидимости в создании сущностей вида FIFO)
		short doc = queue[queue_count - 1];
		queue_count--;

		//обращение к глобальным переменным закончено
		stat = pthread_mutex_unlock(&mutex);
		if(stat != 0)
		{
			fprintf(stderr, "Can't unlock mutex: %s\n", strerror(stat));
			ret = (void*)FAILURE;
			goto scan_thread_ext;
		}
#ifdef __LOG
		printf("Scaner:\tmutex unlocked\n");
#endif

		//ждем положенное время и "сканируем" (печатаем на экран) наш документ
		sleep(*((scan_task_t*)task));
		fprintf(stdout, "Document %02X scanned.\n", doc);
	}

scan_thread_ext:

	return ret;
}

//функция, которую выполняют потоки-клерки
/*принцип работы клерка заключается в следующем:
	1) если клерк еще не подготовил документ, то он его готовит (ждет
		указанное в задании время)
	2) когда документ готов, клерк относит его в очередь на печать (если мест нет,
		то ждет, пока они появятся)
	3) повторяет пункты 1 и 2, пока не подготовит указанное в задание количество
		документов, после чего выходит
*/
void* clerk_thread(void* task)
{
#ifdef __LOG
	printf("Clerk #%d:\tis here, todays task = %d, time for 1 document = %d\n", 
		((clerk_task_t*)task)->id, ((clerk_task_t*)task)->day_task, 
		((clerk_task_t*)task)->doc_prep_time);
#endif
	void* ret = 0;
	
	//делаем документы
	for(int i = 0; i < ((clerk_task_t*)task)->day_task; i++)
	{
		//готовим документ
		sleep(((clerk_task_t*)task)->doc_prep_time);
		short doc = (((clerk_task_t*)task)->id << 4) + i;
	#ifdef __LOG
		printf("Clerk #%d:\tdocument %02x prepared\n", 
			((clerk_task_t*)task)->id, doc);
	#endif

		//пытаемся встать в очередь
	#ifdef __LOG
		printf("Clerk #%d:\ttrying to lock the mutex\n", 
			((clerk_task_t*)task)->id);
	#endif
		int stat = pthread_mutex_lock(&mutex);
		if(stat != 0)
		{
			fprintf(stderr, "Can't lock mutex: %s\n", strerror(stat));
			ret = (void*)FAILURE;
			goto clerk_thread_ext;
		}

	#ifdef __LOG
		printf("Clerk #%d:\tmutex locked\n", ((clerk_task_t*)task)->id);
	#endif

		while(queue_count == max_queue_len)
		{
		#ifdef __LOG
			printf("Clerk #%d:\twaiting for places in queue\n", 
				((clerk_task_t*)task)->id);
		#endif
			stat = pthread_cond_wait(&queue_free_places, &mutex);
			if(stat != 0)
			{
				fprintf(stderr, "Can't perform conditional wait: %s\n",
					strerror(stat));
				ret = (void*)FAILURE;
				goto clerk_thread_ext;
			}
		}

		queue[queue_count] = doc;
		queue_count++;

		if(queue_count == 1)
		{
		#ifdef __LOG
			printf("Clerk #%d:\tbroadcasting the queue not empty\n", 
				((clerk_task_t*)task)->id);
		#endif
			stat = pthread_cond_broadcast(&queue_not_empty);
			if(stat != 0)
			{
				fprintf(stderr, "Can't broadcast condition: %s\n", 
					strerror(stat));
				ret = (void*)FAILURE;
				goto clerk_thread_ext;
			}
		}
		//документ положен в очередь

		stat = pthread_mutex_unlock(&mutex);
		if(stat != 0)
		{
			fprintf(stderr, "Can't unlock mutex: %s\n", strerror(stat));
			ret = (void*)FAILURE;
			goto clerk_thread_ext;
		}
	#ifdef __LOG
		printf("Clerk #%d:\tmutex unlocked\n", ((clerk_task_t*)task)->id);
	#endif
	}
	
clerk_thread_ext:

	return ret;
}
