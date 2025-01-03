#pragma once
#include "GRB.h"
#include "stdafx.h"

//������� ��� ����������� ������ ��������������� ����������� 
#define MFST_TRACE_START(stream) stream<< std::setw(4)<<std::left<<"���"<<":"\
								  << std::setw(20)<<std::left<<" �������"\
								  << std::setw(30)<<std::left<<" ������� �����"\
								  << std::setw(20)<<std::left<<" ����"\
								  << std::endl;

//����� ����, ������� ��������, �������� ����������� ������� ����� � �����
#define MFST_TRACE1(stream)		 stream<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;
//����� ����, �������� ����������� ������� ����� � �����: 
//����� ����������� ������ ������� �������� ��� �������������� ���������
#define MFST_TRACE2(stream)	 stream<< std::setw(4)<<std::left<<FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

//����� ����, �������� ����������� ������� ����� � �����: ������� ��� �����������
#define MFST_TRACE3(stream)		 stream<< std::setw(4)<<std::left<<++FST_TRACE_n<<": "\
								  << std::setw(20)<<std::left<<" "\
								  << std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
								  << std::setw(20)<<std::left<<getCSt(sbuf)\
								  << std::endl;

//����� ��������� � ����������� ����: ������ � ���������� �����������
#define MFST_TRACE4(stream, c)		stream<<std::setw(4)<<std::left << ++FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
//����� ��������� ��� ���������� ����: �������������� ��������� ��������
#define MFST_TRACE5(stream, c)		stream<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left <<c<<std::endl;
//���������� �������� ��������� ��������
#define MFST_TRACE6(stream, c,k)	stream<<std::setw(4)<<std::left << FST_TRACE_n << ": "<<std::setw(20)<< std::left << c << k <<std::endl;
//����� ������ ����������
#define MFST_TRACE7(stream)		stream<<std::setw(4)<<std::left << state.lenta_position << ": "\
							<<std::setw(20)<< std::left << rule.getCRule(rbuf,state.nrulechain)\
							<<std::endl;
#define MFST_DIAGN_MAXSIZE 2* ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

class MFSTSTSTACK :public std::stack<short> {
public:
	using std::stack<short>::c; // ���� ��������
};

static int FST_TRACE_n = -1;	//����� ���� � �����������
static char rbuf[205], sbuf[205], lbuf[1024];	//������ ��� ������ 

namespace MFST
{
	struct MfstState					//��������� �������� ��� ����������
	{
		short lenta_position;			//��������� �������� ��� ����������
		short nrule;					//����� �������� �������
		short nrulechain;				//����� ������� ������, �������� �������
		MFSTSTSTACK st;					//���� ��������
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);	//(������� �� �����;���� ��������; ����� ������� ������� �������� �������)
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);//(������� �� �����;���� ��������; ����� �������� �������; ����� ������� ������� �������� �������)
	};

	//���� ��������
	class MFSTSTSTATE :public std::stack<MfstState> {
	public:
		using std::stack<MfstState>::c;
	};
	struct Mfst							//���������� �������
	{
		enum RC_STEP {					//��� �������� ������� step
			NS_OK,					//������� ������� � �������, ������� �������� � ����
			NS_NORULE,				//�� ������� ������� ����������(������ � ����������)
			NS_NORULECHAIN,			//�� ������� ���������� ������� �������(������ � �������� ����)
			NS_ERROR,				//������������ �������������� ������ ����������
			TS_OK,					//���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,					//���. ������ ����� != ������� �����, ������������ �����, pop �����
			LENTA_END,				//������� ������� ����� >= lenta_size
			SURPRISE,				//����������� ��� �������� (������ � step)
		};
		struct MfstDiagnosis		//�����������
		{
			short lenta_position;		//������� �� �����
			RC_STEP rc_step;			//��� ���������� ����
			short nrule;				//����� �������
			short nrule_chain;			//����� ������� �������
			MfstDiagnosis();			//==
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);

		} diagnosis[MFST_DIAGN_NUMBER]; 		// ��������� ����� �������� ���������

		GRBALPHABET* lenta;					//���������������� (TN/NS) ����� (�� LEX)
		short lenta_position;				//������� ������� �� �����
		short nrule;						//����� �������� �������
		short nrulechain;					//����� ������� �������,�������� �������
		short lenta_size;					//������ �����
		GRB::Greibach grebach;				//���������� �������
		LT::lexTable lex;					//��������� ������ ������������ �����������
		MFSTSTSTACK st;						//���� ��������
		MFSTSTSTATE storestate;		//���� ��� �������� ���������
		Mfst();
		Mfst(LT::lexTable& plex, GRB::Greibach pgrebach);
		char* getCSt(char* buf);			//�������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);	//�����: n �������� � pos
		char* getDiagnosis(short n, char* buf);					//�������� n-� ������ ����������� ��� 0�00
		bool savestate(Log::LOG& log);					//��������� ��������� ��������
		bool reststate(Log::LOG& log);					//������������ ��������� ��������
		bool push_chain(GRB::Rule::Chain chain);		//��������� ������� ������� � ����
		RC_STEP step(Log::LOG& log);						//��������� ��� ��������
		bool start(Log::LOG& log);						//��������� �������
		bool savediagnosis(RC_STEP pprc_step);			//��� ���������� ����
		void printrules(Log::LOG& log);					//������� ������������������ ������

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation()
			{
				size = 0;
				nrules = 0;
				nrulechains = 0;
			};
		}deducation;
		bool savededucation();
	};
}
