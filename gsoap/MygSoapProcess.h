#pragma once
#include <iostream>

//gsoap ҵ���߼�������
//����Ľӿڰ���
/*1.���ģ��
2.ɾ��ģ��
3.1:1�ӿ�
4.1:N�ӿ�
5.��̬1:1�ӿ�
6.��ӿ����
7.ɾ�������
8.��ѯ�����
9.��ѯ������Ϣ*/
//���Ⱪ¶һ��ͳһ�Ľӿ� string FaceServiceAPI(string ReqType,//������� string ReqJson//����json��);
class MygSoapProcess
{
public:
	MygSoapProcess();
	~MygSoapProcess();
	bool FaceServiceAPI(std::string ReqType,/*�������*/ std::string ReqJson/*����json��*/);
	std::string szReqJson;                //���ص�jsonֵ
private:
	wchar_t* StringToWchar_tP(wchar_t * szDst, std::string strDes);           //string תwchar_t
	bool Wchar_tToString(std::string& szDst, wchar_t *DesWchar);
	wchar_t *szDstTemp;                //˽�б���
	char   *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
};

