/*
 * Copyright 2017 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "io_realm_internal_OsList.h"

#include <list.hpp>
#include <results.hpp>
#include <shared_realm.hpp>

#include "util.hpp"
#include "java_accessor_context.hpp"

using namespace realm;
using namespace realm::util;

static void finalize_list(jlong ptr)
{
    TR_ENTER_PTR(ptr)
    delete reinterpret_cast<List*>(ptr);
}

JNIEXPORT jlong JNICALL Java_io_realm_internal_OsList_nativeGetFinalizerPtr(JNIEnv*, jclass)
{
    TR_ENTER()
    return reinterpret_cast<jlong>(&finalize_list);
}

JNIEXPORT jlongArray JNICALL Java_io_realm_internal_OsList_nativeCreate(JNIEnv* env, jclass, jlong shared_realm_ptr,
                                                                        jlong row_ptr, jlong column_index)
{
    TR_ENTER_PTR(row_ptr)

    try {
        auto& row = *reinterpret_cast<realm::Row*>(row_ptr);

        /*
        if (!ROW_AND_COL_INDEX_AND_TYPE_VALID(env, &row, column_index, type_LinkList)) {
            return 0;
        }
        */
        auto& shared_realm = *reinterpret_cast<SharedRealm*>(shared_realm_ptr);
        jlong ret[2];

        auto list_ptr = new List(shared_realm, *row.get_table(), column_index, row.get_index());
        ret[0] = reinterpret_cast<jlong>(list_ptr);

        if (list_ptr->get_type() == PropertyType::Object) {
            LinkViewRef link_view_ref(row.get_linklist(column_index));

            Table* target_table_ptr = &(link_view_ref)->get_target_table();
            LangBindHelper::bind_table_ptr(target_table_ptr);
            ret[1] = reinterpret_cast<jlong>(target_table_ptr);
        }
        else {
            ret[1] = reinterpret_cast<jlong>(nullptr);
        }

        jlongArray ret_array = env->NewLongArray(2);
        if (!ret_array) {
            ThrowException(env, OutOfMemory, "Could not allocate memory to create OsList.");
            return nullptr;
        }
        env->SetLongArrayRegion(ret_array, 0, 2, ret);
        return ret_array;
    }
    CATCH_STD()
    return nullptr;
}

JNIEXPORT jlong JNICALL Java_io_realm_internal_OsList_nativeGetRow(JNIEnv* env, jclass, jlong list_ptr,
                                                                   jlong column_index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        auto row = list.get(column_index);
        return reinterpret_cast<jlong>(new Row(std::move(row)));
    }
    CATCH_STD()
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddRow(JNIEnv* env, jclass, jlong list_ptr,
                                                                  jlong target_row_index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.add(static_cast<size_t>(target_row_index));
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertRow(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                     jlong target_row_index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.insert(static_cast<size_t>(pos), static_cast<size_t>(target_row_index));
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetRow(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                  jlong target_row_index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.set(static_cast<size_t>(pos), static_cast<size_t>(target_row_index));
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeMove(JNIEnv* env, jclass, jlong list_ptr,
                                                                jlong source_index, jlong target_index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.move(source_index, target_index);
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeRemove(JNIEnv* env, jclass, jlong list_ptr, jlong index)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.remove(index);
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeRemoveAll(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.remove_all();
    }
    CATCH_STD()
}

JNIEXPORT jlong JNICALL Java_io_realm_internal_OsList_nativeSize(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        return list.size();
    }
    CATCH_STD()
    return 0;
}

JNIEXPORT jlong JNICALL Java_io_realm_internal_OsList_nativeGetQuery(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        auto query = list.get_query();
        return reinterpret_cast<jlong>(new Query(std::move(query)));
    }
    CATCH_STD()
    return reinterpret_cast<jlong>(nullptr);
}

JNIEXPORT jboolean JNICALL Java_io_realm_internal_OsList_nativeIsValid(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        return list.is_valid();
    }
    CATCH_STD()
    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeDeleteAll(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)

    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        list.delete_all();
    }
    CATCH_STD()
}

template <typename T>
inline void add_value(JNIEnv* env, jlong list_ptr, T&& value)
{
    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        JavaAccessorContext context(env);
        list.add(context, value);
    }
    CATCH_STD()
}

template <typename T>
inline void insert_value(JNIEnv* env, jlong list_ptr, jlong pos, T&& value)
{
    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        JavaAccessorContext context(env);
        list.insert(context, pos, value);
    }
    CATCH_STD()
}

template <typename T>
inline void set_value(JNIEnv* env, jlong list_ptr, jlong pos, T&& value)
{
    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        JavaAccessorContext context(env);
        list.set(context, pos, value);
    }
    CATCH_STD()
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddNull(JNIEnv* env, jclass, jlong list_ptr)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any());
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertNull(JNIEnv* env, jclass, jlong list_ptr, jlong pos)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any());
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetNull(JNIEnv* env, jclass, jlong list_ptr, jlong pos)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any());
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddLong(JNIEnv* env, jclass, jlong list_ptr, jlong value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertLong(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                      jlong value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetLong(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                   jlong value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddDouble(JNIEnv* env, jclass, jlong list_ptr,
                                                                     jdouble value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertDouble(JNIEnv* env, jclass, jlong list_ptr,
                                                                        jlong pos, jdouble value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetDouble(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                     jdouble value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddFloat(JNIEnv* env, jclass, jlong list_ptr, jfloat value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertFloat(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                       jfloat value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetFloat(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                    jfloat value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddBoolean(JNIEnv* env, jclass, jlong list_ptr,
                                                                      jboolean value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertBoolean(JNIEnv* env, jclass, jlong list_ptr,
                                                                         jlong pos, jboolean value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetBoolean(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                      jboolean value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddBinary(JNIEnv* env, jclass, jlong list_ptr,
                                                                     jbyteArray value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertBinary(JNIEnv* env, jclass, jlong list_ptr,
                                                                        jlong pos, jbyteArray value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetBinary(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                     jbyteArray value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddDate(JNIEnv* env, jclass, jlong list_ptr, jlong value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertDate(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                      jlong value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetDate(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                   jlong value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeAddString(JNIEnv* env, jclass, jlong list_ptr,
                                                                     jstring value)
{
    TR_ENTER_PTR(list_ptr)
    add_value(env, list_ptr, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeInsertString(JNIEnv* env, jclass, jlong list_ptr,
                                                                        jlong pos, jstring value)
{
    TR_ENTER_PTR(list_ptr)
    insert_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT void JNICALL Java_io_realm_internal_OsList_nativeSetString(JNIEnv* env, jclass, jlong list_ptr, jlong pos,
                                                                     jstring value)
{
    TR_ENTER_PTR(list_ptr)
    set_value(env, list_ptr, pos, Any(value));
}

JNIEXPORT jobject JNICALL Java_io_realm_internal_OsList_nativeGetValue(JNIEnv* env, jclass, jlong list_ptr, jlong pos)
{
    TR_ENTER_PTR(list_ptr)
    try {
        auto& list = *reinterpret_cast<List*>(list_ptr);
        JavaAccessorContext context(env);
        return any_cast<jobject>(list.get(context, pos));
    }
    CATCH_STD()

    return nullptr;
}
