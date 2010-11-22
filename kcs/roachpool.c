#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>
#include <errno.h>

#include <sys/types.h>

#include <katcp.h>
#include <katpriv.h>

#include "kcs.h"


int roachpool_greeting(struct katcp_dispatch *d){
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"add [roach hostname] [roach ip] [pool type]");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"del [roach hostname | pool type]");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"mod [roach hostname] [new pool type]");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST, "list");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"start [roach hostname]");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"stop [roach hostname]");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"start-pool");
  prepend_inform_katcp(d);
  append_string_katcp(d, KATCP_FLAG_STRING | KATCP_FLAG_LAST ,"stop-pool");
  return KATCP_RESULT_OK;
}

int roachpool_add(struct katcp_dispatch *d){

  return KATCP_RESULT_OK;
}
int roachpool_mod(struct katcp_dispatch *d){

  return KATCP_RESULT_OK;
}
int roachpool_del(struct katcp_dispatch *d){

  return KATCP_RESULT_OK;
}
int roachpool_list(struct katcp_dispatch *d){

  return KATCP_RESULT_OK;
}
int roachpool_destroy(struct katcp_dispatch *d){

  return KATCP_RESULT_OK;
}

#if 0
struct kcs_obj *init_kcs_tree(){
  
  struct kcs_obj *o;
  
  o = malloc(sizeof(struct kcs_obj));

  if (!o) return NULL;

  o->tid      = KCS_ID_GENERIC;
  o->parent   = NULL;
  o->pool     = NULL;
  o->payload  = NULL;

#ifdef DEBUG
  fprintf(stderr,"KCS init tree root at (%p)\n",o);
#endif
  return o;
}

struct kcs_obj *create_kcs_tree_node(char *name){
  
}

struct kcs_obj *create_kcs_tree_leaf(void *payload){

}

int add_kcs_tree_obj(struct kcs_obj *parent, struct kcs_obj *child){

}

int del_kcs_tree_obj(struct kcs_obj *obj){

}

struct kcs_obj *find_kcs_tree_obj(char *sstr){

}

int destroy_kcs_tree(struct kcs_obj *root){

}

static struct kcs_tree_operations kcs_tree_funcs[] = { 
  [KCS_ID_GENERIC] = {
    &init_kcs_tree,
    NULL,
    NULL,
    NULL,
    &del_kcs_tree_obj,
    &find_kcs_tree_obj,
    &destroy_kcs_tree_obj
  },
  [KCS_ID_NODE] = {
    NULL,
    &create_kcs_tree_node,
    &create_kcs_tree_leaf,
    &add_kcs_tree_obj,
    NULL,
    NULL,
    NULL
  },
  [KCS_ID_ROACH] = {
    NULL,
    NULL,
    &create_kcs_tree_leaf,
    NULL,
    NULL,
    NULL,
    NULL
  }
};




/*
void add_roach_to_node(struct kcs_node *cn, struct kcs_roach *nr){
#ifdef DEBUG
  fprintf(stderr,"Added roach %p to node %s (%p)\n",nr,cn->desc,cn);
#endif
  nr->parent = cn;
  cn->childroaches = realloc(cn->childroaches,sizeof(struct kcs_roach*)*(++cn->childcount));
  cn->childroaches[cn->childcount-1] = nr;
}

struct kcs_obj * search_tree(struct kcs_ojb *o, char *s){
  int i;
  struct kcs_obj *ko;

  //struct kcs_node *c;
  //struct kcs_roach *r;

  if (n == NULL)
    return NULL;

#ifdef DEBUG
  fprintf(stderr,"SEARCH INSIDE node %s: (%p) cc:%d\n",n->desc,n,n->childcount);
#endif

  if (strcmp(o->pool,s) != 0){
    if (n->childnodes != NULL){
      for (i=0;i<n->childcount;i++){
        c = n->childnodes[i];
        c = search_tree(c,s);
        if (c == NULL){
#ifdef DEBUG
          fprintf(stderr,"SEARCH GOT A NULL\n");
#endif
        }
        else
          return c;
      }
    }
    if (n->childroaches != NULL){
      for (i=0;i<n->childcount;i++){
        r = n->childroaches[i];
        if (strcmp(r->hostname,s) == 0){
#ifdef DEBUG
          fprintf(stderr,"SEARCH found roach with hostname: %s\n",s);
#endif
          return r;
        }
      }
    }
  } else {
#ifdef DEBUG
    fprintf(stderr,"SEARCH FOUND node pool: %s\n",s);
#endif
    return n;
  }
   
  return NULL;
}

struct kcs_ojb * create_new_node(char *desc){
  struct kcs_ojb *ko;
  struct kcs_node *kn;
  ko                 = malloc(sizeof(struct kcs_obj));
  if (ko == NULL) return NULL;
  kn                 = malloc(sizeof(struct kcs_node));
  if (kn == NULL) { free(ko); return NULL; }
  
  ko->tid          = KCS_ID_NODE;
  ko->parent       = NULL;
  ko->pool         = malloc(sizeof(char)*strlen(desc));
  ko->pool         = strcpy(ko->pool,desc);
  ko->payload      = kn;

  kn->children     = NULL;
  kn->childcount   = 0;

#ifdef DEBUG
  fprintf(stderr,"Created new node: %s: %p\n",desc,ko);
#endif
  
  return ko;
}

void add_node_to_tree(struct kcs_node *p, struct kcs_node *n){
  
  p->childnodes = realloc(p->childnodes,sizeof(struct kcs_node*)*(++p->childcount));
  p->childnodes[p->childcount-1] = n;
  n->parent = p;

#ifdef DEBUG
  fprintf(stderr,"ADD_NODE_TO_TREE: parent:%s (%p) child:%s (%p)\n",p->desc,p,n->desc,n);
#endif

}

struct kcs_node * find_root(struct kcs_node *n){
#ifdef DEBUG
  fprintf(stderr,"FIND_PARENT in: %s\n",n->desc);
#endif
  if (n->parent == NULL)
    return n;
  return find_root(n->parent);
}

struct kcs_node * create_sub_node(struct kcs_node *n, char *nndesc){
  
  struct kcs_node *cn,*pn;
  
  if (n->childnodes == NULL){
    //node has no children
#ifdef DEBUG
    fprintf(stderr,"NODE: %s has no childnodes\n",n->desc);
#endif
    if (n->childroaches == NULL){
      //node has no roaches node can be root
#ifdef DEBUG
      fprintf(stderr,"NODE: %s has no childroaches\n",n->desc);
#endif
      return n;
    } else{
      //node has roaches so we need a new root
#ifdef DEBUG
      fprintf(stderr,"NODE: %s has roaches so we need to restructure\n",n->desc);
#endif
      pn = create_new_node("root");
      add_node_to_tree(pn,n);
      cn = create_new_node(nndesc);
      add_node_to_tree(pn,cn);
      return cn;
    }
  } else {
#ifdef DEBUG
    fprintf(stderr,"NODE: %s has children so we just add another\n",n->desc);
#endif
    cn = create_new_node(nndesc);
    add_node_to_tree(n,cn);
    return cn;
  }
}

struct kcs_ojb * place_node_in_tree(struct kcs_ojb *co, struct kcs_ojb *no){
  
  struct kcs_ojb *o;
  
  if (co == NULL){
    
    o = create_new_node(no->pool);
    if (o == NULL)
      retiurn NULL;
    
    return place_node_in_tree(o,no);

  } else {
    
    o = search_tree(co,no->pool);
    if (o == NULL){
      o = create_sub_node(co,no->pool);
      return place_node_in_tree(o,no);
    }
    
    add_roach_to_node(o,no);
    
  }

  return find_root(o);
}
*/

int roachpool_add(struct katcp_dispatch *d){

  struct kcs_basic *kb;
  struct kcs_obj *ko;
  /*struct kcs_roach *kr;
*/
  
  kb = need_current_mode_katcp(d, KCS_MODE_BASIC);
  
  ko = kb->b_pool_head;

  if (!ko){
    ko = (*(kcs_tree_funcs[KCS_ID_GENERIC].t_init))();
    kb->b_pool_head = ko;
  }
  
  
  
  
  
  /*
  ko           = malloc(sizeof(struct kcs_obj));
  if (ko == NULL) return KATCP_RESULT_FAIL;
  kr           = malloc(sizeof(struct kcs_roach));
  if (kr == NULL) { free(ko); return KATCP_RESULT_FAIL;}
  
  ko->tid      = KCS_ID_ROACH;
  ko->parent   = NULL;
  ko->pool     = arg_copy_string_katcp(d,4);

  ko->payload  = kr;

  kr->hostname = arg_copy_string_katcp(d,2);
  kr->ip       = arg_copy_string_katcp(d,3);

#ifdef DEBUG
  fprintf(stderr,"NEW ROACH: %s %s pool:%s\n",kr->hostname,kr->ip,ko->pool);
#endif

  kb->b_pool_head = place_node_in_tree(kb->b_pool_head,ko);
*/
  return KATCP_RESULT_OK;
}

/*
void traverse_tree(struct katcp_dispatch *d, struct kcs_node *n){
  
  int i;
  struct kcs_roach *r;
  struct kcs_node *c;
  
  if (n->childcount > 0){
    if (n->childnodes == NULL){
      for (i=0; i<n->childcount; i++){
        r = n->childroaches[i];
        prepend_inform_katcp(d);
        append_string_katcp(d,KATCP_FLAG_STRING, r->type);
        append_string_katcp(d,KATCP_FLAG_STRING, r->hostname);
        append_string_katcp(d,KATCP_FLAG_STRING | KATCP_FLAG_LAST, r->ip);

      }
    } else if (n->childroaches == NULL){
#ifdef DEBUG
      fprintf(stderr,"Traversing children of node: %s\n",n->desc);
#endif
      prepend_inform_katcp(d);
      append_string_katcp(d,KATCP_FLAG_STRING, "Traversing node:");
      append_string_katcp(d,KATCP_FLAG_STRING | KATCP_FLAG_LAST, n->desc);
      for (i=0; i<n->childcount; i++) {
        c = n->childnodes[i];
        traverse_tree(d,c);
      }
    }
  }
#ifdef DEBUG
  else {
    fprintf(stderr,"NO children of node: %s\n",n->desc);
  }
#endif
}
*/

int roachpool_list(struct katcp_dispatch *d){

  struct kcs_basic *kb;
  
  kb = need_current_mode_katcp(d,KCS_MODE_BASIC);
  traverse_tree(d,kb->b_pool_head);

  //prepend_reply_katcp(d);
  //append_string_katcp(d,KATCP_FLAG_STRING,KATCP_OK);
  //append_unsigned_long_katcp(d,KATCP_FLAG_ULONG | KATCP_FLAG_LAST, i);
  
  return KATCP_RESULT_OWN;
}


int roachpool_mod(struct katcp_dispatch *d) {
  
  struct kcs_basic *kb;
  struct kcs_roach *r;
  char *sroach, *newtype;

  kb = need_current_mode_katcp(d,KCS_MODE_BASIC);
  
  sroach = arg_string_katcp(d,2);
  newtype = arg_string_katcp(d,3);

  r = (struct kcs_roach *) search_tree(kb->b_pool_head,sroach);
  if (r == NULL){
#ifdef DEBUG
    fprintf(stderr,"Cannot find roach with hostname: %s\n",sroach);
#endif
    return KATCP_RESULT_FAIL;
  } else {
#ifdef DEBUG
    fprintf(stderr,"Found roach: %s (%p)\n",r->hostname,r);
#endif  
  }

  return KATCP_RESULT_OK;
}


int roachpool_del(struct katcp_dispatch *d){

  struct kcs_basic *kb;
  void *sresult;
  char *sroach;

  kb = need_current_mode_katcp(d,KCS_MODE_BASIC);

  sroach = arg_string_katcp(d,2);
    
}

/*
void destroy_tree(struct kcs_node *n){
  
  int i;
  struct kcs_roach *r;
  struct kcs_node *c;
  
  if (n->childcount > 0){
    if (n->childnodes == NULL){
      for (i=0; i<n->childcount; i++){
        r = n->childroaches[i];
#ifdef DEBUG
        fprintf(stderr,"Freeing roach %s\n",r->hostname);
#endif  
        //if (r->parent)   { free(r->parent);   r->parent = NULL; }
        if (r->hostname) { free(r->hostname); r->hostname = NULL; }
        if (r->ip)       { free(r->ip);       r->ip = NULL; }
        if (r->mac)      { free(r->mac);      r->mac = NULL; }
        if (r->type)     { free(r->type);     r->type = NULL; }
        free(r);
      }
      if (n->childroaches) { free(n->childroaches); n->childroaches = NULL; }
    } else if (n->childroaches == NULL){
      for (i=0; i<n->childcount; i++) {
        c = n->childnodes[i];
        destroy_tree(c);
      }
      if (n->childnodes) { free(n->childnodes); n->childnodes = NULL; }
    }
  }
#ifdef DEBUG
  else {
    fprintf(stderr,"NO children of node: %s\n",n->desc);
  }
  fprintf(stderr,"Freeing node %s\n",n->desc);
#endif  
  free(n);
  n = NULL;
}
*/
int roachpool_destroy(struct katcp_dispatch *d){
  
  struct kcs_basic *kb;
  
  kb = need_current_mode_katcp(d,KCS_MODE_BASIC);
  destroy_tree(kb->b_pool_head);

  return KATCP_RESULT_OK;
}

#endif
