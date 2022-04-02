#include <stdio.h>
#include <string.h>

#define LINE_MAX 1024

/**
 *@para   path �����ļ�·��
 *@return �ļ�����
 */
int read_line(char *path)
{
	FILE *fp;
	int line_num = 0; // �ļ�����
	int line_len = 0; // �ļ�ÿ�еĳ���
	char buf[LINE_MAX] = {0}; // �����ݻ���

	fp = fopen(path, "r");
	if (NULL == fp) {
		printf("open %s failed.\n", path);
		return -1;
	}

	while(fgets(buf, LINE_MAX, fp)) {
		line_num++;
		line_len = strlen(buf);
		// �ų����з�
		if ('\n' == buf[line_len - 1]) {
			buf[line_len - 1] = '\0';
			line_len--;
			if (0 == line_len) {
				//����
				continue;
			}
		}
		// windos�ı��ų��س���
		if ('\r' == buf[line_len - 1]) {
			buf[line_len - 1] = '\0';
			line_len--;
			if (0 == line_len) {
				//����
				continue;
			}
		}
		printf("%s\n", buf);
		/** ��ÿ������(buf)���д��� **/
	}

	if (0 == feof) {
		// δ�����ļ�ĩβ
		printf("fgets error\n");
		return -1;
	}
	fclose(fp);

	return line_num;
}
