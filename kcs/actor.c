/* (c) 2010,2011 SKA SA */
/* Released under the GNU GPLv3 - see COPYING */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <search.h>

#include <katcp.h>
#include <katcl.h>
#include <katpriv.h>

#include "kcs.h"


struct katcp_actor *create_actor_type_katcp(struct katcp_dispatch *d, char *str, struct katcp_job *j, struct katcp_notice *n, void *data, char *datatype)
{
  struct katcp_actor *a;

  a = malloc(sizeof(struct katcp_actor));
  if (a == NULL)
    return NULL;

  a->a_data = NULL;

  a->a_key = strdup(str);
  if(a->a_key == NULL){
    destroy_actor_type_katcp(a);
    return NULL;
  }

  a->a_job = j;
  a->a_sm_notice = n;
  
  if (data != NULL && datatype != NULL){
    a->a_data = create_named_tobject_katcp(d, data, datatype, 0);
  }

  a->a_tag_root = NULL;
  a->a_tag_count = 0;

  return a;
}

int assign_sm_notice_actor_type_katcp(struct katcp_actor *a, struct katcp_notice *n)
{
  if (a == NULL || n == NULL)
    return -1;
  
  a->a_sm_notice = n;
  
  return 0;
}

#if 0
void walk_actor_tags(const void *nodep, const VISIT which, const int depth)
{
  struct katcp_tag *t;

  t = *(struct katcp_tag **) nodep;
  if (t == NULL)
    return;

  switch (which){
    case leaf:
#if 0
    def DEBUG
      fprintf(stderr, "\tleaf:");
#endif
    case postorder:
#ifdef DEBUG
      fprintf(stderr, "\t<%s;%d>\n", t->t_name, t->t_level);
#endif
      break;
    case preorder:
    case endorder:
      break;
  }
}
#endif

void print_actor_type_katcp(struct katcp_dispatch *d, void *data)
{
  struct katcp_actor *a;
  a = data;
  if (a == NULL)
    return;

  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_FIRST, "#actor type:");
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST, a->a_key);

#if 0
def DEBUG
  fprintf(stderr, "actor: print %s with %d tags\n", a->a_key, a->a_tag_count);
  if (a->a_tag_root != NULL)
    twalk(a->a_tag_root, walk_actor_tags);
  fprintf(stderr, "actor: end\n");
#endif

}

void do_nothing(void *data)
{
#ifdef DEBUG
  fprintf(stderr, "actor: do_nothing (%p)\n", data);
#endif
}

/*Unsafe*/
void destroy_actor_type_katcp(void *data)
{
  struct katcp_actor *a;

  a = data;
  if (a == NULL)
    return;
  if (a->a_key) { free(a->a_key); }

  a->a_job = NULL;
  a->a_sm_notice = NULL;
  if (a->a_tag_root != NULL) 
    tdestroy(a->a_tag_root, do_nothing);

  destroy_tobject_katcp(a->a_data);

  free(a);
}

int copy_actor_type_katcp(void *src, void *dest, int n)
{

  return 0;
}

int compare_actor_type_katcp(const void *a, const void *b)
{

  return 0;
}

void *parse_actor_type_katcp(struct katcp_dispatch *d, char **str)
{
  struct katcp_actor *a;
  
  a = create_actor_type_katcp(d, str[0], NULL, NULL, NULL, NULL);
  
  return a;
}

char *getkey_actor_katcp(void *data)
{
  struct katcp_actor *a;
  a = data;
  if (a == NULL)
    return NULL;
  return a->a_key;
}

struct katcp_tag *create_tag_katcp(char *name, int level)
{
  struct katcp_tag *t;
  
  t = malloc(sizeof(struct katcp_tag));
  if (t == NULL)
    return NULL;

  t->t_name = strdup(name);
  if (t->t_name == NULL){
    free(t);
    return NULL;
  }

  t->t_level = level;
  t->t_tobject_root = NULL;
  t->t_tobject_count = 0;

#if 0
  t->t_memb = NULL;
  t->t_memb_count = 0;
#endif

  return t;
}

/*Unsafe*/
void destroy_tag_katcp(void *data)
{
  struct katcp_tag *t;
  t = data;
  if (t == NULL)
    return;
  
  if (t->t_name != NULL) free(t->t_name);
#if 0
  if (t->t_memb != NULL) free(t->t_memb);
#endif
  if (t->t_tobject_root != NULL) 
    tdestroy(t->t_tobject_root, &destroy_tobject_katcp);

  free(t);
}

#if 0
void walk_tag_tobjects(const void *nodep, const VISIT which, const int depth)
{
  struct katcp_tobject *to;

  to = *(struct katcp_tobject **) nodep;
  if (to == NULL)
    return;

  switch (which){
    case leaf:
#ifdef DEBUG
      fprintf(stderr, "\tleaf:");
#endif
    case postorder:
#ifdef DEBUG
      fprintf(stderr, "\ttobject data: %p and type: %p mem_manage: %d\n", to->o_data, to->o_type, to->o_man);
#endif
      break;
    case preorder:
    case endorder:
      break;
  }
}
#endif

void print_tag_katcp(struct katcp_dispatch *d, void *data)
{
  struct katcp_tag *t;
  t = data;
  if (t == NULL)
    return;

  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_FIRST, "#tag type:");
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST, t->t_name);
  //append_args_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST, "%s", t->t_memb_count);
  
#if 0 
  def DEBUG
  fprintf(stderr, "tag: %s has %d tobjects:\n", t->t_name, t->t_tobject_count);
  if (t->t_tobject_root)
    twalk(t->t_tobject_root, &walk_tag_tobjects);
  fprintf(stderr, "tag: end\n");
#endif
}

void *parse_tag_katcp(struct katcp_dispatch *d, char **str)
{
  struct katcp_tag *t;
  int level;

  level = 0;

  if (str == NULL || str[0] == NULL || str[1] == NULL)
    return NULL;

  level = atoi(str[1]);

  t = create_tag_katcp(str[0], level);
  
  return t;
}

char *getkey_tag_katcp(void *data)
{
  struct katcp_tag *t;
  t = data;
  if (t == NULL)
    return NULL;
  return t->t_name;
}

int compare_tag_katcp(const void *m1, const void *m2)
{
  const struct katcp_tag *a, *b;

  a = m1;
  b = m2;
  if (a == NULL || b == NULL)
    return 2;

  return strcmp(a->t_name, b->t_name);
}

int register_tag_katcp(struct katcp_dispatch *d, char *name, int level)
{
  struct katcp_tag *t;

  t = create_tag_katcp(name, level);
  if (t == NULL)
    return -1;

  if (store_data_type_katcp(d, KATCP_TYPE_TAG, KATCP_DEP_BASE, name, t, &print_tag_katcp, &destroy_tag_katcp, NULL, NULL, &parse_tag_katcp, &getkey_tag_katcp) < 0){
    destroy_tag_katcp(d);
    return -1;
  }

#ifdef DEBUG
  fprintf(stderr, "tag: register tag <%s>\n", name);
#endif
  return 0;
}

struct katcp_tobject **__tobjs;
int __tcount;

void collect_tobjects_from_tag(const void *nodep, const VISIT which, const int depth)
{
  struct katcp_tobject *to;

  to = *(struct katcp_tobject **) nodep;
  if (to == NULL)
    return;

  switch (which){
    case leaf:
    case postorder:
      if (__tobjs == NULL)
        return;
      __tobjs[__tcount] = to;
      __tcount++;
      break;
    case preorder:
    case endorder:
      break;
  }
}

void destroy_tobjs_katcp()
{
  free(__tobjs);
  __tobjs   = NULL;
  __tcount  = 0;
}

int populate_tobjs_katcp(struct katcp_tag *t)
{
  if (t == NULL)
    return -1;

  if (__tobjs != NULL)
    destroy_tobjs_katcp();

  __tobjs   = malloc(sizeof(struct katcp_tobject *) * t->t_tobject_count);
  __tcount  = 0;
  
  if (t->t_tobject_root != NULL)
    twalk(t->t_tobject_root, &collect_tobjects_from_tag);
  
  if (__tobjs == NULL)
    return -1;

  return 0;
}

int deregister_tag_katcp(struct katcp_dispatch *d, char *name)
{
  struct katcp_tag *t;
  struct katcp_tobject *to;
  struct katcp_type *actor_type;
  int i;

  if (name == NULL)
    return -1;
  
  t = get_key_data_type_katcp(d, KATCP_TYPE_TAG, name);
  if (t == NULL)
    return -1;

  if (populate_tobjs_katcp(t) < 0)
    return -1;
  
  actor_type = find_name_type_katcp(d, KATCP_TYPE_ACTOR);

  for (i=0; i<__tcount; i++){
    to = __tobjs[i];
    if (to != NULL){
      /*TODO: a type specific detagger method*/  
      if (to->o_type == actor_type){
        untag_actor_katcp(d, to->o_data, t);
      }
    }
  }
  
  destroy_tobjs_katcp();

  return del_data_type_katcp(d, KATCP_TYPE_TAG, name);
}

struct katcp_tag **__tags;

void collect_tags_from_actor(const void *nodep, const VISIT which, const int depth)
{
  struct katcp_tag *t;

  t = *(struct katcp_tag **) nodep;
  if (t == NULL)
    return;

  switch (which){
    case leaf:
    case postorder:
      if (__tags == NULL)
        return;
      __tags[__tcount] = t;
      __tcount++;
      break;
    case preorder:
    case endorder:
      break;
  }
}

int unlink_tags_actor_katcp(struct katcp_dispatch *d, struct katcp_actor *a)
{
  struct katcp_tag *t;
  int i;
  
  if (a == NULL)
    return -1;
  
  __tags = malloc(sizeof(struct katcp_tag *) * a->a_tag_count);
  __tcount = 0;
  
  if (a->a_tag_root != NULL)
    twalk(a->a_tag_root, &collect_tags_from_actor);
  
  if (__tags == NULL)
    return -1;
  
  for (i=0; i<__tcount; i++){
    t = __tags[i];
    if (t != NULL){
      untag_actor_katcp(d, a, t);
    }
  }

  free(__tags);
  __tags   = NULL;
  __tcount = 0;
  
#ifdef DEBUG
  fprintf(stderr, "actor: unlink tags for <%s> complete\n", a->a_key);
#endif

  return 0;
}

int add_tobject_tag_katcp(struct katcp_dispatch *d, struct katcp_tag *t, struct katcp_actor *a, char *type)
{
  struct katcp_tobject *to;
  void *val;

  if (t == NULL || a == NULL || type == NULL)
    return -1;
  
  to = create_named_tobject_katcp(d, a, type, 0);
  if (to == NULL)
    return -1;

  val = tfind((void *) to, &(t->t_tobject_root), &compare_tobject_katcp);
  if (val != NULL){
#ifdef DEBUG
    fprintf(stderr, "tag: tag already contains tobject with corresponding data\n");
#endif
    destroy_tobject_katcp(to);
    return -1;
  }

  val = tsearch((void *) to, &(t->t_tobject_root), &compare_tobject_katcp);
  if (val == NULL || ( *(struct katcp_tobject **) val != to )){
    log_message_katcp(d, KATCP_LEVEL_ERROR, NULL, "tag: error with tsearch for actor:<%s> in tag:<%s>", a->a_key, t->t_name);
#ifdef DEBUG
    fprintf(stderr, "tag: error with tsearch for actor:<%s> in tag:<%s>\n", a->a_key, t->t_name);
#endif
    destroy_tobject_katcp(to);
    return -1;
  }
  
#if 0
  log_message_katcp(d, KATCP_LEVEL_INFO, NULL, "added tobject <%s> to tag <%s>", a->a_key, t->t_name);
#endif

  t->t_tobject_count++;

  return 0;
}

int del_tobject_tag_katcp(struct katcp_dispatch *d, struct katcp_tag *t, struct katcp_actor *a, char *type)
{
  struct katcp_tobject *to;
  void *val;

  if (t == NULL || a == NULL || type == NULL)
    return -1;
  
  to = create_named_tobject_katcp(d, a, type, 0);
  if (to == NULL)
    return -1;

  val = tfind((void *) to, &(t->t_tobject_root), &compare_tobject_katcp);
  if (val == NULL){
#ifdef DEBUG
    fprintf(stderr, "tag: could not find tobject to delete\n");
#endif
    return -1;
  }

  destroy_tobject_katcp(to);

  to = *(struct katcp_tobject **) val;
  
  val = tdelete((void *) to, &(t->t_tobject_root), &compare_tobject_katcp);
  if (val == NULL){
#ifdef DEBUG
    fprintf(stderr, "tag: could not find tobject to delete\n");
#endif
    destroy_tobject_katcp(to);
    return -1;
  }

  destroy_tobject_katcp(to);

  log_message_katcp(d, KATCP_LEVEL_INFO, NULL, "deleted tobject <%s> from tag <%s>", a->a_key, t->t_name);

  t->t_tobject_count--;

  return 0;
}

int tag_actor_katcp(struct katcp_dispatch *d, struct katcp_actor *a, struct katcp_tag *t)
{
  void *val;
 
  val = tsearch((void *) t, &(a->a_tag_root), &compare_tag_katcp);
  if (val == NULL || ( *(struct katcp_tag **) val != t )){
    log_message_katcp(d, KATCP_LEVEL_ERROR, NULL, "error with tsearch for tag: <%s> in: <%s>", t->t_name, a->a_key);
#ifdef DEBUG
    fprintf(stderr, "actor: error with tsearch for tag:<%s>  in:<%s>\n", t->t_name, a->a_key);
#endif
    return -1;
  }
  
  if (add_tobject_tag_katcp(d, t, a, KATCP_TYPE_ACTOR) < 0){
#ifdef DEBUG
    fprintf(stderr, "actor: cannot add actor to tag set. undo\n");
#endif
    //untag_actor_katcp(d, a, t);
    return -1;
  }

  log_message_katcp(d, KATCP_LEVEL_INFO, NULL, "tagged actor <%s> with <%s>", a->a_key, t->t_name);

#ifdef DEBUG
  fprintf(stderr, "actor: tagged actor <%s> with <%s>\n", a->a_key, t->t_name);
#endif
  
  a->a_tag_count++;

  return 0;  
}

int tag_named_actor_katcp(struct katcp_dispatch *d, struct katcp_actor *a, char *tag)
{ 
  struct katcp_tag *t;

  if (a == NULL || tag == NULL)
    return -1;
  
  t = get_key_data_type_katcp(d, KATCP_TYPE_TAG, tag);
  if (t == NULL)
    return -1;

  return tag_actor_katcp(d, a, t);
}

int untag_actor_katcp(struct katcp_dispatch *d, struct katcp_actor *a, struct katcp_tag *t)
{
  void *val;

  val = tdelete((void *)t, &(a->a_tag_root), &compare_tag_katcp);
  if (val == NULL){
#ifdef DEBUG
    fprintf(stderr, "actor: untag actor tdelete returned NULL\n");
#endif
    return -1;
  }

  if (del_tobject_tag_katcp(d, t, a, KATCP_TYPE_ACTOR) < 0){
#ifdef DEBUG
    fprintf(stderr, "actor: cannot del actor tobject from tag set\n");
#endif
    return -1;
  }

  log_message_katcp(d, KATCP_LEVEL_ERROR, NULL, "untagged actor <%s> with <%s>", a->a_key, t->t_name);

#ifdef DEBUG
  fprintf(stderr, "actor: untagged actor <%s> with <%s>\n", a->a_key, t->t_name);
#endif
  
  a->a_tag_count--;

  return 0;
}

int untag_named_actor_katcp(struct katcp_dispatch *d, struct katcp_actor *a, char *tag)
{
  struct katcp_tag *t;

  if (a == NULL || tag == NULL)
    return -1;
  
  t = get_key_data_type_katcp(d, KATCP_TYPE_TAG, tag);
  if (t == NULL)
    return -1;
  
  return untag_actor_katcp(d, a, t);
}


int tag_actor_sm_katcp(struct katcp_dispatch *d, struct katcp_stack *stack, struct katcp_tobject *o)
{
  struct katcp_actor *a;
  struct katcp_tag *t;
  int rtn;

  if (stack == NULL)
    return -1;

  a = pop_data_expecting_stack_katcp(d, stack, KATCP_TYPE_ACTOR);
  if (a == NULL)
    return -1;

  rtn = 0;

  while ((t = pop_data_expecting_stack_katcp(d, stack, KATCP_TYPE_TAG)) != NULL){
    rtn += tag_actor_katcp(d, a, t);
  }

  return rtn;
}

struct kcs_sm_op *tag_actor_sm_setup_katcp(struct katcp_dispatch *d, struct kcs_sm_state *s)
{
  return create_sm_op_kcs(&tag_actor_sm_katcp, NULL);
}

int get_tag_set_sm_katcp(struct katcp_dispatch *d, struct katcp_stack *stack, struct katcp_tobject *o)
{
  struct katcp_tag *t;
  int rtn, i;

  rtn = 0;

  if (stack == NULL)
    return -1;

  t = pop_data_expecting_stack_katcp(d, stack, KATCP_TYPE_TAG);
  if (t == NULL)
    return -1;

  if (populate_tobjs_katcp(t) < 0)
    return -1;
  
  for (i=0; i<__tcount; i++){
    rtn += push_tobject_katcp(stack, copy_tobject_katcp(__tobjs[i]));
  }
  
  destroy_tobjs_katcp();
  
  return rtn;
}

struct kcs_sm_op *get_tag_set_sm_setup_katcp(struct katcp_dispatch *d, struct kcs_sm_state *s)
{
  return create_sm_op_kcs(&get_tag_set_sm_katcp, NULL);
}

int init_actor_tag_katcp(struct katcp_dispatch *d)
{
  int rtn;
  
  rtn  = register_name_type_katcp(d, KATCP_TYPE_ACTOR, KATCP_DEP_BASE, &print_actor_type_katcp, &destroy_actor_type_katcp, &copy_actor_type_katcp, &compare_actor_type_katcp, &parse_actor_type_katcp, &getkey_actor_katcp);
  
  rtn += register_name_type_katcp(d, KATCP_TYPE_TAG, KATCP_DEP_BASE, &print_tag_katcp, &destroy_tag_katcp, NULL, &compare_tag_katcp, &parse_tag_katcp, &getkey_tag_katcp);

  rtn += store_data_type_katcp(d, KATCP_TYPE_OPERATION, KATCP_DEP_BASE, KATCP_OPERATION_TAG_ACTOR, &tag_actor_sm_setup_katcp, NULL, NULL, NULL, NULL, NULL, NULL);

  rtn += store_data_type_katcp(d, KATCP_TYPE_OPERATION, KATCP_DEP_BASE, KATCP_OPERATION_GET_TAG_SET, &get_tag_set_sm_setup_katcp, NULL, NULL, NULL, NULL, NULL, NULL);

  return rtn;
}

#ifdef __ACTOR_UNIT_TEST
int main(int argc, char *argv[])
{
  struct katcp_dispatch *d;
  struct katcp_actor *a1, *a2, *a3, *a4;
  
  int rtn;

  d = startup_katcp();
  if (d == NULL)
    return 1;

  rtn = register_name_type_katcp(d, KATCP_TYPE_ACTOR, KATCP_DEP_BASE, &print_actor_type_katcp, &destroy_actor_type_katcp, &copy_actor_type_katcp, &compare_actor_type_katcp, &parse_actor_type_katcp, &getkey_actor_katcp);

  a1 = create_actor_type_katcp(d, "actor1", NULL, NULL, NULL, NULL); 
  a2 = create_actor_type_katcp(d, "actor2", NULL, NULL, NULL, NULL); 
  a3 = create_actor_type_katcp(d, "actor3", NULL, NULL, NULL, NULL); 
  a4 = create_actor_type_katcp(d, "actor4", NULL, NULL, NULL, NULL); 

  rtn += register_tag_katcp(d, "tag1", 0);
  rtn += register_tag_katcp(d, "tag2", 0);
  rtn += register_tag_katcp(d, "tag3", 0);
  rtn += register_tag_katcp(d, "tag4", 0);
 
  rtn += tag_named_actor_katcp(d, a1, "tag1");
  rtn += tag_named_actor_katcp(d, a1, "tag2");
  rtn += tag_named_actor_katcp(d, a1, "tag3");
  rtn += tag_named_actor_katcp(d, a1, "tag4");
  
  rtn += tag_named_actor_katcp(d, a2, "tag1");
  rtn += tag_named_actor_katcp(d, a2, "tag2");
  rtn += tag_named_actor_katcp(d, a2, "tag3");
  rtn += tag_named_actor_katcp(d, a2, "tag4");
  
  rtn += tag_named_actor_katcp(d, a3, "tag1");
  rtn += tag_named_actor_katcp(d, a3, "tag2");
  rtn += tag_named_actor_katcp(d, a3, "tag3");
  rtn += tag_named_actor_katcp(d, a3, "tag4");
 
  rtn += tag_named_actor_katcp(d, a4, "tag1");
  rtn += tag_named_actor_katcp(d, a4, "tag2");
  rtn += tag_named_actor_katcp(d, a4, "tag3");
  rtn += tag_named_actor_katcp(d, a4, "tag4");
  
  print_actor_type_katcp(d, a1);
  rtn += unlink_tags_actor_katcp(d, a1); 
  destroy_actor_type_katcp(a1);
  a1 = NULL;

  print_actor_type_katcp(d, a1);
  print_actor_type_katcp(d, a2);
  print_actor_type_katcp(d, a3);
  print_actor_type_katcp(d, a4);
  
#if 0 
  rtn += deregister_tag_katcp(d, "tag1");
  rtn += deregister_tag_katcp(d, "tag2");
  rtn += deregister_tag_katcp(d, "tag3");
  rtn += deregister_tag_katcp(d, "tag4");

  rtn += untag_named_actor_katcp(d, a1, "tag1");
  rtn += untag_named_actor_katcp(d, a1, "tag2");
  rtn += untag_named_actor_katcp(d, a1, "tag3");
  rtn += untag_named_actor_katcp(d, a1, "tag4") 
#endif
  
  destroy_actor_type_katcp(a2);
  destroy_actor_type_katcp(a3);
  destroy_actor_type_katcp(a4);
  
  shutdown_katcp(d);

  return rtn;
}
#endif
