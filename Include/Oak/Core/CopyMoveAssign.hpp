
#pragma once

#ifndef OAK_DISALLOW_COPY_ASSIGN

#define OAK_DISALLOW_COPY_ASSIGN(typeName) \
    typeName(const typeName&) = delete;    \
    typeName& operator=(const typeName&) = delete;

#endif // OAK_DISABLE_COPY_ASSIGN

#ifndef OAK_DISALLOW_MOVE_ASSIGN

#define OAK_DISALLOW_MOVE_ASSIGN(typeName) \
    typeName(typeName&&) = delete;         \
    typeName& operator=(typeName&&) = delete;

#endif // OAK_DISABLE_COPY_ASSIGN

#ifndef OAK_DISALLOW_COPY_MOVE_ASSIGN

#define OAK_DISALLOW_COPY_MOVE_ASSIGN(typeName) \
    OAK_DISALLOW_COPY_ASSIGN(typeName);         \
    OAK_DISALLOW_MOVE_ASSIGN(typeName);

#endif // OAK_DISALLOW_COPY_MOVE_ASSIGN
