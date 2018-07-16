

        /**
         * adb_getStatus2.c
         *
         * This file was auto-generated from WSDL
         * by the Apache Axis2/C version: SNAPSHOT  Built on : Mar 10, 2008 (08:35:52 GMT+00:00)
         */

        #include "adb_getStatus2.h"
        
               /*
                * implmentation of the getStatus|http://esb.presenceagent.services.hdviper.psnc.pl/ element
                */
           


        struct adb_getStatus2
        {
            
                axutil_qname_t* qname;
            adb_getStatus_t* property_getStatus;

                
                axis2_bool_t is_valid_getStatus;


            
        };


       /************************* Private Function prototypes ********************************/
        

                axis2_status_t AXIS2_CALL
                adb_getStatus2_set_getStatus_nil(
                        adb_getStatus2_t* _getStatus2,
                        const axutil_env_t *env);
            


       /************************* Function Implmentations ********************************/
        adb_getStatus2_t* AXIS2_CALL
        adb_getStatus2_create(
            const axutil_env_t *env)
        {
            adb_getStatus2_t *_getStatus2 = NULL;
            
                axutil_qname_t* qname = NULL;
            
            AXIS2_ENV_CHECK(env, NULL);

            _getStatus2 = (adb_getStatus2_t *) AXIS2_MALLOC(env->
                allocator, sizeof(adb_getStatus2_t));

            if(NULL == _getStatus2)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return NULL;
            }

            memset(_getStatus2, 0, sizeof(adb_getStatus2_t));

            _getStatus2->property_getStatus  = NULL;
                  _getStatus2->is_valid_getStatus  = AXIS2_FALSE;
            
                  qname =  axutil_qname_create (env,
                        "getStatus",
                        "http://esb.presenceagent.services.hdviper.psnc.pl/",
                        NULL);
                _getStatus2->qname = qname;
            

            return _getStatus2;
        }

        axis2_status_t AXIS2_CALL
        adb_getStatus2_free (
                adb_getStatus2_t* _getStatus2,
                const axutil_env_t *env)
        {
            

            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            AXIS2_PARAM_CHECK(env->error, _getStatus2, AXIS2_FAILURE);

            adb_getStatus2_reset_getStatus(_getStatus2, env);
            
              if(_getStatus2->qname)
              {
                  axutil_qname_free (_getStatus2->qname, env);
                  _getStatus2->qname = NULL;
              }
            

            if(_getStatus2)
            {
                AXIS2_FREE(env->allocator, _getStatus2);
                _getStatus2 = NULL;
            }
            return AXIS2_SUCCESS;
        }


        

        axis2_status_t AXIS2_CALL
        adb_getStatus2_deserialize(
                adb_getStatus2_t* _getStatus2,
                const axutil_env_t *env,
                axiom_node_t **dp_parent,
                axis2_bool_t *dp_is_early_node_valid,
                axis2_bool_t dont_care_minoccurs)
        {
          axiom_node_t *parent = *dp_parent;
          
          axis2_status_t status = AXIS2_SUCCESS;
          
              void *element = NULL;
           
             axis2_char_t* text_value = NULL;
             axutil_qname_t *qname = NULL;
          
            axutil_qname_t *element_qname = NULL; 
            
               axiom_node_t *first_node = NULL;
               axis2_bool_t is_early_node_valid = AXIS2_TRUE;
               axiom_node_t *current_node = NULL;
               axiom_element_t *current_element = NULL;
            
            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            AXIS2_PARAM_CHECK(env->error, _getStatus2, AXIS2_FAILURE);

            
              
              while(parent && axiom_node_get_node_type(parent, env) != AXIOM_ELEMENT)
              {
                  parent = axiom_node_get_next_sibling(parent, env);
              }
              if (NULL == parent)
              {
                /* This should be checked before everything */
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                            "Failed in building adb object for getStatus : "
                            "NULL elemenet can not be passed to deserialize");
                return AXIS2_FAILURE;
              }
              

                    current_element = (axiom_element_t *)axiom_node_get_data_element(parent, env);
                    qname = axiom_element_get_qname(current_element, env, parent);
                    if (axutil_qname_equals(qname, env, _getStatus2-> qname))
                    {
                        
                          first_node = parent;
                          
                    }
                    else
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                              "Failed in building adb object for getStatus : "
                              "Expected %s but returned %s",
                              axutil_qname_to_string(_getStatus2-> qname, env),
                              axutil_qname_to_string(qname, env));
                        
                        return AXIS2_FAILURE;
                    }
                    

                     
                     /*
                      * building getStatus element
                      */
                     
                     
                     
                                   current_node = first_node;
                                   is_early_node_valid = AXIS2_FALSE;
                                   
                                   
                                    while(current_node && axiom_node_get_node_type(current_node, env) != AXIOM_ELEMENT)
                                    {
                                        current_node = axiom_node_get_next_sibling(current_node, env);
                                    }
                                    if(current_node != NULL)
                                    {
                                        current_element = (axiom_element_t *)axiom_node_get_data_element(current_node, env);
                                        qname = axiom_element_get_qname(current_element, env, current_node);
                                    }
                                   
                                 element_qname = axutil_qname_create(env, "getStatus", "http://esb.presenceagent.services.hdviper.psnc.pl/", NULL);
                                 

                           if (adb_getStatus_is_particle() ||  
                                (current_node   && current_element && (axutil_qname_equals(element_qname, env, qname))))
                           {
                              if( current_node   && current_element && (axutil_qname_equals(element_qname, env, qname)))
                              {
                                is_early_node_valid = AXIS2_TRUE;
                              }
                              
                                 
                                      element = (void*)adb_getStatus_create(env);

                                      status =  adb_getStatus_deserialize((adb_getStatus_t*)element,
                                                                            env, &current_node, &is_early_node_valid, AXIS2_FALSE);
                                      if(AXIS2_FAILURE == status)
                                      {
                                          AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building adb object for element getStatus");
                                      }
                                      else
                                      {
                                          status = adb_getStatus2_set_getStatus(_getStatus2, env,
                                                                   (adb_getStatus_t*)element);
                                      }
                                    
                                 if(AXIS2_FAILURE ==  status)
                                 {
                                     AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in setting the value for getStatus ");
                                     if(element_qname)
                                     {
                                         axutil_qname_free(element_qname, env);
                                     }
                                     return AXIS2_FAILURE;
                                 }
                              }
                           
                              else if(!dont_care_minoccurs)
                              {
                                  if(element_qname)
                                  {
                                      axutil_qname_free(element_qname, env);
                                  }
                                  /* this is not a nillable element*/
                                  AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "non nillable or minOuccrs != 0 element getStatus missing");
                                  return AXIS2_FAILURE;
                              }
                           
                  if(element_qname)
                  {
                     axutil_qname_free(element_qname, env);
                     element_qname = NULL;
                  }
                 
          return status;
       }

          axis2_bool_t AXIS2_CALL
          adb_getStatus2_is_particle()
          {
            
                 return AXIS2_FALSE;
              
          }


          void AXIS2_CALL
          adb_getStatus2_declare_parent_namespaces(
                    adb_getStatus2_t* _getStatus2,
                    const axutil_env_t *env, axiom_element_t *parent_element,
                    axutil_hash_t *namespaces, int *next_ns_index)
          {
            
                  /* Here this is an empty function, Nothing to declare */
                 
          }

        
        
        axiom_node_t* AXIS2_CALL
        adb_getStatus2_serialize(
                adb_getStatus2_t* _getStatus2,
                const axutil_env_t *env, axiom_node_t *parent, axiom_element_t *parent_element, int parent_tag_closed, axutil_hash_t *namespaces, int *next_ns_index)
        {
            
            
         
         axiom_node_t *current_node = NULL;
         int tag_closed = 0;

         
         
                axiom_namespace_t *ns1 = NULL;

                axis2_char_t *qname_uri = NULL;
                axis2_char_t *qname_prefix = NULL;
                axis2_char_t *p_prefix = NULL;
                axis2_bool_t ns_already_defined;
            
                    axis2_char_t text_value_1[64];
                    
               axis2_char_t *start_input_str = NULL;
               axis2_char_t *end_input_str = NULL;
               unsigned int start_input_str_len = 0;
               unsigned int end_input_str_len = 0;
            
            
               axiom_data_source_t *data_source = NULL;
               axutil_stream_t *stream = NULL;

             
                int next_ns_index_value = 0;
            

            AXIS2_ENV_CHECK(env, NULL);
            AXIS2_PARAM_CHECK(env->error, _getStatus2, NULL);
            
             
                    namespaces = axutil_hash_make(env);
                    next_ns_index = &next_ns_index_value;
                     
                           ns1 = axiom_namespace_create (env,
                                             "http://esb.presenceagent.services.hdviper.psnc.pl/",
                                             "n"); 
                           axutil_hash_set(namespaces, "http://esb.presenceagent.services.hdviper.psnc.pl/", AXIS2_HASH_KEY_STRING, axutil_strdup(env, "n"));
                       
                     
                    parent_element = axiom_element_create (env, NULL, "getStatus", ns1 , &parent);
                    
                    
                    axiom_element_set_namespace(parent_element, env, ns1, parent);


            
                    data_source = axiom_data_source_create(env, parent, &current_node);
                    stream = axiom_data_source_get_stream(data_source, env);
                  
                       if(!(p_prefix = (axis2_char_t*)axutil_hash_get(namespaces, "http://esb.presenceagent.services.hdviper.psnc.pl/", AXIS2_HASH_KEY_STRING)))
                       {
                           p_prefix = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof (axis2_char_t) * ADB_DEFAULT_NAMESPACE_PREFIX_LIMIT);
                           sprintf(p_prefix, "n%d", (*next_ns_index)++);
                           axutil_hash_set(namespaces, "http://esb.presenceagent.services.hdviper.psnc.pl/", AXIS2_HASH_KEY_STRING, p_prefix);
                           
                           axiom_element_declare_namespace_assume_param_ownership(parent_element, env, axiom_namespace_create (env,
                                            "http://esb.presenceagent.services.hdviper.psnc.pl/",
                                            p_prefix));
                       }
                      

                   if (!_getStatus2->is_valid_getStatus)
                   {
                      
                            
                            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Nil value found in non-nillable property getStatus");
                            return NULL;
                          
                   }
                   else
                   {
                     start_input_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) *
                                 (4 + axutil_strlen(p_prefix) + 
                                  axutil_strlen("getStatus"))); 
                                 
                                 /* axutil_strlen("<:>") + 1 = 4 */
                     end_input_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) *
                                 (5 + axutil_strlen(p_prefix) + axutil_strlen("getStatus")));
                                  /* axutil_strlen("</:>") + 1 = 5 */
                                  
                     

                   
                   
                     
                     /*
                      * parsing getStatus element
                      */

                    
                    
                            sprintf(start_input_str, "<%s%sgetStatus",
                                 p_prefix?p_prefix:"",
                                 (p_prefix && axutil_strcmp(p_prefix, ""))?":":""); 
                            
                        start_input_str_len = axutil_strlen(start_input_str);
                        sprintf(end_input_str, "</%s%sgetStatus>",
                                 p_prefix?p_prefix:"",
                                 (p_prefix && axutil_strcmp(p_prefix, ""))?":":"");
                        end_input_str_len = axutil_strlen(end_input_str);
                    
                            adb_getStatus_serialize(_getStatus2->property_getStatus, 
                                                                                 env, current_node, parent_element,
                                                                                 adb_getStatus_is_particle() || AXIS2_TRUE, namespaces, next_ns_index);
                            
                     
                     AXIS2_FREE(env->allocator,start_input_str);
                     AXIS2_FREE(env->allocator,end_input_str);
                 } 

                 
                   if(namespaces)
                   {
                       axutil_hash_index_t *hi;
                       void *val;
                       for (hi = axutil_hash_first(namespaces, env); hi; hi = axutil_hash_next(env, hi)) 
                       {
                           axutil_hash_this(hi, NULL, NULL, &val);
                           AXIS2_FREE(env->allocator, val);
                       }
                       axutil_hash_free(namespaces, env);
                   }
                

            return parent;
        }


        

            /**
             * getter for getStatus.
             */
            adb_getStatus_t* AXIS2_CALL
            adb_getStatus2_get_getStatus(
                    adb_getStatus2_t* _getStatus2,
                    const axutil_env_t *env)
             {
                
                    AXIS2_ENV_CHECK(env, NULL);
                    AXIS2_PARAM_CHECK(env->error, _getStatus2, NULL);
                  

                return _getStatus2->property_getStatus;
             }

            /**
             * setter for getStatus
             */
            axis2_status_t AXIS2_CALL
            adb_getStatus2_set_getStatus(
                    adb_getStatus2_t* _getStatus2,
                    const axutil_env_t *env,
                    adb_getStatus_t*  arg_getStatus)
             {
                

                AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
                AXIS2_PARAM_CHECK(env->error, _getStatus2, AXIS2_FAILURE);
                
                if(_getStatus2->is_valid_getStatus &&
                        arg_getStatus == _getStatus2->property_getStatus)
                {
                    
                    return AXIS2_SUCCESS; 
                }

                
                  if(NULL == arg_getStatus)
                  {
                      AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "getStatus is being set to NULL, but it is not a nullable element");
                      return AXIS2_FAILURE;
                  }
                adb_getStatus2_reset_getStatus(_getStatus2, env);

                
                if(NULL == arg_getStatus)
                {
                    /* We are already done */
                    return AXIS2_SUCCESS;
                }
                _getStatus2->property_getStatus = arg_getStatus;
                        _getStatus2->is_valid_getStatus = AXIS2_TRUE;
                    
                return AXIS2_SUCCESS;
             }

             

           /**
            * resetter for getStatus
            */
           axis2_status_t AXIS2_CALL
           adb_getStatus2_reset_getStatus(
                   adb_getStatus2_t* _getStatus2,
                   const axutil_env_t *env)
           {
               int i = 0;
               int count = 0;
               void *element = NULL;

               AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
               AXIS2_PARAM_CHECK(env->error, _getStatus2, AXIS2_FAILURE);
               

               
            
                
                if(_getStatus2->property_getStatus != NULL)
                {
                   
                   
                        adb_getStatus_free(_getStatus2->property_getStatus, env);
                     _getStatus2->property_getStatus = NULL;
                }
            
                
                
                _getStatus2->is_valid_getStatus = AXIS2_FALSE; 
               return AXIS2_SUCCESS;
           }

           /**
            * Check whether getStatus is nill
            */
           axis2_bool_t AXIS2_CALL
           adb_getStatus2_is_getStatus_nil(
                   adb_getStatus2_t* _getStatus2,
                   const axutil_env_t *env)
           {
               AXIS2_ENV_CHECK(env, AXIS2_TRUE);
               AXIS2_PARAM_CHECK(env->error, _getStatus2, AXIS2_TRUE);
               
               return !_getStatus2->is_valid_getStatus;
           }

           /**
            * Set getStatus to nill (currently the same as reset)
            */
           axis2_status_t AXIS2_CALL
           adb_getStatus2_set_getStatus_nil(
                   adb_getStatus2_t* _getStatus2,
                   const axutil_env_t *env)
           {
               return adb_getStatus2_reset_getStatus(_getStatus2, env);
           }

           

