TDataSetFilter
��������� ��� ����� ���������� � ������������ ��������� � DataSet ��� ������� ������ � ��������.

�����:
 V.S.Ovchinnikov
 utnpsys@gmail.com
 2017


������� ������ � �����������

1. ������� ��������� (��������, ���������� �� ����� ��� � DataModule)

2. ��������� �������� ��������� DataSet - ��� ����� ������, � ������� �� ��������� ������������ ����������

3. � ����������� ����

3.1 ��������� ������� �� ��������� �������
 
    filter->DisableControls();

3.2 �������� �������� �������
    filter->add("filterName", "filterText");

    // ������:

    filter->add("AcctIdComboBox", "acct_id like '%:param%'");
    filter->add("AddressComboBox", "address like '%:param%'");
    filter->add("CityComboBox", "city like '%:param%'");
    filter->add("FioComboBox", "fio like '%:param%'");
    filter->add("FaPackIdFilterEdit", "fa_pack_id like '%:param%'");
    filter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
    filter->add("SaldoFilterEdit", "saldo_uch > :param");
    filter->add("OpAreaDescrFilterComboBox", "op_area_descr like '%:param%'");
    filter->add("CcDttmStatusComboBox", "(:param = 0 or (:param=1 and cc_dttm is not null) or (:param=2 and cc_dttm is null) or (:param=3 and cc_dttm >= ':begin_dt' and cc_dttm < ':end_dt') )");

3.3 ��������� �������� �� ��������� (�� �����������)
 
    filter->setDefaultValue("CcDttmStatusComboBox", "param", "0");
    filter->setDefaultValue("CcDttmStatusComboBox", "begin_dt", beginDt);
    filter->setDefaultValue("CcDttmStatusComboBox", "end_dt", endDt);


3.4 ��������� ������� �� ��������� �������

    filter->EnableControls();

3.5 ����������������� � ������� ��������� �������� ��������� �������
    filter->setValue("filterName", "paramName", "paramValue");

    ������:
    filter->setValue("AcctIdComboBox", "param", editBox->Text);

3.6 ����������������� � ������� ������ �������
    filter->clearAllValues();

