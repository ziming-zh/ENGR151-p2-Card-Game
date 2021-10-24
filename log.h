//
// Created by Ziming on 2021/10/16.
//

#ifndef P2_1_LOG_H
#define P2_1_LOG_H

FILE* create_log(char *filename, FILE *log);
void read_log(char *filename);

void close_log(FILE *log);


#endif //P2_1_LOG_H
