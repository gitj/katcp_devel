#ifndef _KATPRIV_H_
#define _KATPRIV_H_

#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>

#define KATCP_NAME_LENGTH     64

#define KATCL_IO_SIZE       4096  /* block we want to write out */
#define KATCL_BUFFER_INC     512  /* amount by which we resize read */
#define KATCL_ARGS_INC         8  /* grow the vector by this amount */

#define KATCL_PARSE_FRESH      0  /* newly allocated or cleared */
#define KATCL_PARSE_COMMAND    1  /* parsing first argument */
#define KATCL_PARSE_WHITESPACE 2  /* parsing between arguments */
#define KATCL_PARSE_ARG        3  /* parsing argument */
#define KATCL_PARSE_TAG        4  /* parsing optional tag */
#define KATCL_PARSE_ESCAPE     5  /* parsing escape sequence */
#define KATCL_PARSE_FAKE       6  /* generated manually, not parsed */
#define KATCL_PARSE_DONE       7  /* a complete message */

/***************************************************************************/

struct katcl_larg{
  unsigned int a_begin;
  unsigned int a_end;
  unsigned int a_escape;
};

struct katcl_queue
{
  struct katcl_parse **q_queue; /* parse array */
  unsigned int q_size;          /* size of queue */
  unsigned int q_head;          /* current position */
  unsigned int q_count;         /* No of entries */
};

#if 0
struct katcl_msg{
  struct katcl_line *m_line;

  char *m_buffer;
  unsigned int m_size;
  unsigned int m_want;
  int m_tag;
  int m_complete;
};
#endif

struct katcl_parse{
  unsigned int p_magic;
  unsigned int p_state;

  char *p_buffer;
  unsigned int p_size;
  unsigned int p_have;
  unsigned int p_used;
  unsigned int p_kept;

  struct katcl_larg *p_args;
  struct katcl_larg *p_current;
  unsigned int p_count;
  unsigned int p_got;

  int p_refs;
  int p_tag;
};

struct katcl_line{
  int l_fd;

  struct katcl_parse *l_ready;
  struct katcl_parse *l_next;

  struct katcl_parse *l_stage;

  char l_buffer[KATCL_IO_SIZE];
  unsigned int l_pending;
  unsigned int l_arg;  /* argument */
  unsigned int l_offset; /* offset into argument */

  struct katcl_queue *l_queue;

  int l_error;
};

struct katcp_dispatch;

struct katcp_cmd{
  char *c_name;
  char *c_help;
  int (*c_call)(struct katcp_dispatch *d, int argc);
  struct katcp_cmd *c_next;
  unsigned int c_mode;
  unsigned int c_flags;
};

/**********************************************************************/

#define KATCP_SENSOR_INVALID (-1)
#define KATCP_SENSOR_INTEGER  0 
#define KATCP_SENSOR_BOOLEAN  1
#define KATCP_SENSOR_DISCRETE 2
#define KATCP_SENSOR_LRU      3

#ifdef KATCP_USE_FLOATS
#define KATCP_SENSOR_FLOAT    4
#define KATCP_SENSORS_COUNT   5
#else
#define KATCP_SENSORS_COUNT   4
#endif

struct katcp_sensor;
struct katcp_nonsense;
struct katcp_acquire;

struct katcp_integer_acquire{
  int ia_current;
  int (*ia_get)(struct katcp_dispatch *d, struct katcp_acquire *a);
};

struct katcp_acquire{
  struct katcp_sensor **a_sensors;
  unsigned int a_count;

  int a_type;
  int a_users;
  int a_periodics;

  struct timeval a_poll;    /* rate at which we poll this sensor */
  struct timeval a_current; /* current rate */
  struct timeval a_limit;   /* fastest update rate */
  struct timeval a_last;    /* last time value was acquired */

  void *a_local;
  void (*a_release)(struct katcp_dispatch *d, struct katcp_acquire *a);

  void *a_more; /* could be a union */
};

struct katcp_sensor{
  int s_magic;
  int s_type;
  char *s_name;
  char *s_description;
  char *s_units;

  int s_preferred;

  int s_status; /* WARNING, etc */
  int s_mode; /* in what mode available */
  struct timeval s_recent;
#if 0
  int s_running; /* have it do stuff */
#endif

  unsigned int s_refs;
  struct katcp_nonsense **s_nonsense;

  struct katcp_acquire *s_acquire;

  int (*s_extract)(struct katcp_dispatch *d, struct katcp_sensor *sn);
  void *s_more;
};

struct katcp_integer_sensor{
  int is_current;
  int is_min;
  int is_max;
};

struct katcp_nonsense{
  int n_magic;
  struct katcp_dispatch *n_client;
  struct katcp_sensor *n_sensor;
  int n_strategy;
  int n_status;
  struct timeval n_period;
  struct timeval n_next;
  int n_manual;

  void *n_more;
};

struct katcp_integer_nonsense{
  int in_previous;
  int in_delta;
};

#if 0
struct katcp_sensor_integer{
  int si_min;
  int si_max;
  int si_current;
  int (*si_get)(struct katcp_sensor *s, void *local);
};

struct katcp_sensor_discrete{
  int sd_current;
  char **sd_vector;
  unsigned int sd_size;
  int (*sd_get)(struct katcp_sensor *s, void *local);
};

struct katcp_nonsense_discrete{
  int nd_previous;
};

#ifdef KATCP_USE_FLOATS
struct katcp_sensor_float{
  double sf_min;
  double sf_max;
  double sf_current;
  double (*sf_get)(struct katcp_sensor *s, void *local);
};

struct katcp_nonsense_float{
  double nf_previous;
  double nf_delta;
};
#endif
#endif

/**********************************************************************/

struct katcp_entry{
  char *e_name;
  int (*e_enter)(struct katcp_dispatch *d, char *flags, unsigned int mode);
  void (*e_leave)(struct katcp_dispatch *d, unsigned int mode);
  void *e_state;
  void (*e_clear)(struct katcp_dispatch *d);
  char *e_version;
  unsigned int e_minor;
  unsigned int e_major;
};

#if 0
#define KATCP_PS_UP    1
#define KATCP_PS_TERM  2
struct katcp_process{
  void (*p_call)(struct katcp_dispatch *d, int status);
  pid_t p_pid;
  char *p_name;
  int p_type;
  int p_state;
};
#endif

struct katcp_notice;

struct katcp_trap{
  char *t_name;
  struct katcp_notice *t_notice;
};

struct katcp_map{
  struct katcp_trap **m_traps;
  unsigned int m_size;
};

struct katcp_job{
  unsigned int j_magic;
  char *j_name;

  pid_t j_pid;

  int j_state; /* state machine */
  int j_code; /* exit code */

  struct katcl_line *j_line;

  struct katcp_notice *j_halt;

  struct katcp_notice **j_queue; 
  unsigned int j_size;
  unsigned int j_head; /* points at the current head */
  unsigned int j_count; /* number of entries present */
};

#if 0
#define KATCP_TIME_OTHER   0
#define KATCP_TIME_CURRENT 1
#define KATCP_TIME_REFRESH 2
#define KATCP_TIME_REAP    3
#endif

struct katcp_time{
  int t_magic;

  struct timeval t_when;
  struct timeval t_interval;

  int t_armed;

  void *t_data;
  int (*t_call)(struct katcp_dispatch *d, void *data);
};

struct katcp_invoke{
  struct katcp_dispatch *v_client;
  void *v_data;
  int (*v_call)(struct katcp_dispatch *d, struct katcp_notice *n, void *data);
};

struct katcp_notice{
  struct katcp_invoke *n_vector;
  unsigned int n_count;

  int n_trigger;
  int n_code;
  char *n_name;

  int n_tag;
  int n_use;

  /* might get away with only one parse structure */
  struct katcl_parse *n_parse;

#if 0
  void *n_target;
  int (*n_release)(struct katcp_dispatch *d, struct katcp_notice *n, void *target);
#endif
};

struct katcp_shared{
  unsigned int s_magic;
  struct katcp_entry *s_vector;
  unsigned int s_size;
  unsigned int s_modal;

  struct katcp_cmd *s_commands;
  unsigned int s_mode;

  struct katcp_dispatch *s_template;
  struct katcp_dispatch **s_clients;

  unsigned int s_count;
  unsigned int s_used;

  int s_lfd;

  struct katcp_job **s_tasks;
  unsigned int s_number;

#if 0
  struct katcp_process *s_table;
  int s_entries;
#endif

  struct katcp_time **s_queue;
  unsigned int s_length;

  struct katcp_notice **s_notices;
  unsigned int s_pending;

#if 0
  int s_version_major;
  int s_version_minor;
  char *s_version_subsystem; /* not ideally named */
#endif

  char **s_build_state;
  int s_build_items;

  struct katcp_sensor **s_sensors;
  unsigned int s_tally;

  sigset_t s_mask_current, s_mask_previous;
  struct sigaction s_action_current, s_action_previous;
  int s_restore_signals;

  fd_set s_read, s_write;
  int s_max;
};

struct katcp_dispatch{
  int d_level; /* log level */
  int d_ready;
  int d_run; /* 1 if up, -1 if shutting down, 0 if shut down */
  int d_exit; /* exit code, reason for shutting down */
  int d_pause; /* waiting for a notice */
  struct katcl_line *d_line;

  int (*d_current)(struct katcp_dispatch *d, int argc);

  struct katcp_shared *d_shared;

  struct katcp_nonsense **d_nonsense;
  unsigned int d_size;

  struct katcp_notice **d_notices;
  unsigned int d_count;

  struct katcp_notice *d_end;

  int d_clone;

  char d_name[KATCP_NAME_LENGTH];
};

void exchange_katcl(struct katcl_line *l, int fd);

int dispatch_cmd_katcp(struct katcp_dispatch *d, int argc);

void component_time_katcp(struct timeval *result, unsigned int ms);
int sub_time_katcp(struct timeval *delta, struct timeval *alpha, struct timeval *beta);
int add_time_katcp(struct timeval *sigma, struct timeval *alpha, struct timeval *beta);
int cmp_time_katcp(struct timeval *alpha, struct timeval *beta);

int startup_shared_katcp(struct katcp_dispatch *d);
void shutdown_shared_katcp(struct katcp_dispatch *d);
int listen_shared_katcp(struct katcp_dispatch *d, int count, char *host, int port);
int link_shared_katcp(struct katcp_dispatch *d, struct katcp_dispatch *cd);

int load_shared_katcp(struct katcp_dispatch *d);
int run_shared_katcp(struct katcp_dispatch *d);
int ended_shared_katcp(struct katcp_dispatch *d);

void shutdown_cmd_katcp(struct katcp_cmd *c);

int sensor_value_cmd_katcp(struct katcp_dispatch *d, int argc);
int sensor_list_cmd_katcp(struct katcp_dispatch *d, int argc);
int sensor_sampling_cmd_katcp(struct katcp_dispatch *d, int argc);
int sensor_dump_cmd_katcp(struct katcp_dispatch *d, int argc);
int sensor_cmd_katcp(struct katcp_dispatch *d, int argc);

char *code_to_name_katcm(int code);

/* timing support */
int empty_timers_katcp(struct katcp_dispatch *d);
int run_timers_katcp(struct katcp_dispatch *d, struct timespec *interval);
void dump_timers_katcp(struct katcp_dispatch *d);

/* nonsense support */
void forget_nonsense_katcp(struct katcp_dispatch *d, unsigned int index);

/* how many times to try waitpid for child to exit */
#define KATCP_WAITPID_CHECKS 5 
/* how long to sleep between checks in nanoseconds */
#define KATCP_WAITPID_POLL   250000000UL

int child_signal_shared_katcp(struct katcp_shared *s);

int reap_children_shared_katcp(struct katcp_dispatch *d, pid_t pid, int force);
int init_signals_shared_katcp(struct katcp_shared *s);
int undo_signals_shared_katcp(struct katcp_shared *s);

/* notice logic */
void disown_notices_katcp(struct katcp_dispatch *d);
void destroy_notices_katcp(struct katcp_dispatch *d);
int run_notices_katcp(struct katcp_dispatch *d);
int notice_cmd_katcp(struct katcp_dispatch *d, int argc);
int cancel_notice_katcp(struct katcp_dispatch *d, struct katcp_notice *n);

/* jobs */
int load_jobs_katcp(struct katcp_dispatch *d);
int wait_jobs_katcp(struct katcp_dispatch *d);
int run_jobs_katcp(struct katcp_dispatch *d);

int job_cmd_katcp(struct katcp_dispatch *d, int argc);
int register_subprocess_cmd_katcp(struct katcp_dispatch *d, int argc);
int submit_to_job_katcp(struct katcp_dispatch *d, struct katcp_job *j, struct katcl_parse *p, char *name, int (*call)(struct katcp_dispatch *d, struct katcp_notice *n));
int ended_jobs_katcp(struct katcp_dispatch *d);

/* parse: setup */
struct katcl_parse *create_parse_katcl();
void destroy_parse_katcl(struct katcl_parse *p);
struct katcl_parse *reuse_parse_katcl(struct katcl_parse *p);
struct katcl_parse *copy_parse_katcl(struct katcl_parse *p);
struct katcl_parse *turnaround_parse_katcl(struct katcl_parse *p, int code);

/* parse: adding fields */
int add_plain_parse_katcl(struct katcl_parse *p, int flags, char *string);
int add_string_parse_katcl(struct katcl_parse *p, int flags, char *buffer);
int add_unsigned_long_parse_katcl(struct katcl_parse *p, int flags, unsigned long v);
int add_signed_long_parse_katcl(struct katcl_parse *p, int flags, unsigned long v);
int add_hex_long_parse_katcl(struct katcl_parse *p, int flags, unsigned long v);
#ifdef KATCP_USE_FLOATS
int add_double_parse_katcl(struct katcl_parse *p, int flags, double v);
#endif
int add_buffer_parse_katcl(struct katcl_parse *p, int flags, void *buffer, unsigned int len);
int add_parameter_parse_katcl(struct katcl_parse *pd, int flags, struct katcl_parse *ps, unsigned int index);

/* parse: extracting, testing fields */
unsigned int get_count_parse_katcl(struct katcl_parse *p);
int get_tag_parse_katcl(struct katcl_parse *p);

int is_type_parse_katcl(struct katcl_parse *p, char mode);
int is_request_parse_katcl(struct katcl_parse *p);
int is_reply_parse_katcl(struct katcl_parse *p);
int is_inform_parse_katcl(struct katcl_parse *p);
int is_null_parse_katcl(struct katcl_parse *p, unsigned int index);

char *get_string_parse_katcl(struct katcl_parse *p, unsigned int index);
char *copy_string_parse_katcl(struct katcl_parse *p, unsigned int index);
unsigned long get_unsigned_long_parse_katcl(struct katcl_parse *p, unsigned int index);
#ifdef KATCP_USE_FLOATS
double get_double_parse_katcl(struct katcl_parse *p, unsigned int index);
#endif
unsigned int get_buffer_parse_katcl(struct katcl_parse *p, unsigned int index, void *buffer, unsigned int size);

/* parse: parsing from line */
int parse_katcl(struct katcl_line *l);
struct katcl_parse *ready_katcl(struct katcl_line *l);

#include <stdarg.h>

int add_vargs_parse_katcl(struct katcl_parse *p, int flags, char *fmt, va_list args);
int add_args_parse_katcl(struct katcl_parse *p, int flags, char *fmt, ...);

int dump_parse_katcl(struct katcl_parse *p, char *prefix, FILE *fp);

/*Queue logic*/
struct katcl_parse *get_head_katcl(struct katcl_queue *q);
struct katcl_parse *get_tail_katcl(struct katcl_queue *q);
struct katcl_queue *create_queue_katcl(void);
void destroy_queue_katcl(struct katcl_queue *q);
void clear_queue_katcl(struct katcl_queue *q);
unsigned int is_empty_queue_katcl(struct katcl_queue *q);

int add_tail_queue(struct katcl_queue *q, struct katcl_parse *p);
struct katcl_parse *remove_index_queue(struct katcl_queue *q, unsigned int index);
struct katcl_parse *remove_head_queue(struct katcl_queue *q);
void dump_queue_parse_katcp(struct katcl_queue *q, FILE *fp);

/******************************************/

#define KATCL_PARSE_MAGIC 0xff7f1273

#endif
