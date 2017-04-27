TDataSetFilter
Компонент для связи визуальных и невизуальных элементов с DataSet для удобной работы с фильтром.

Автор:
 V.S.Ovchinnikov
 utnpsys@gmail.com
 2017


Порядок работы с компонентом

1. Создать компонент (например, разместить на форме или в DataModule)

2. Назначить значение параметра DataSet - это набор данных, в котором мы планируем осуществлять фильтрацию

3. В программном коде

3.1 Запретить реакцию на изменение фильтра
 
    filter->DisableControls();

3.2 Добавить элементы фильтра
    filter->add("filterName", "filterText");

    // Пример:

    filter->add("AcctIdComboBox", "acct_id like '%:param%'");
    filter->add("AddressComboBox", "address like '%:param%'");
    filter->add("CityComboBox", "city like '%:param%'");
    filter->add("FioComboBox", "fio like '%:param%'");
    filter->add("FaPackIdFilterEdit", "fa_pack_id like '%:param%'");
    filter->add("ServiceCompanyFilterComboBox", "service_org like '%:param%'");
    filter->add("SaldoFilterEdit", "saldo_uch > :param");
    filter->add("OpAreaDescrFilterComboBox", "op_area_descr like '%:param%'");
    filter->add("CcDttmStatusComboBox", "(:param = 0 or (:param=1 and cc_dttm is not null) or (:param=2 and cc_dttm is null) or (:param=3 and cc_dttm >= ':begin_dt' and cc_dttm < ':end_dt') )");

3.3 Назначить значения по умолчанию (не обязательно)
 
    filter->setDefaultValue("CcDttmStatusComboBox", "param", "0");
    filter->setDefaultValue("CcDttmStatusComboBox", "begin_dt", beginDt);
    filter->setDefaultValue("CcDttmStatusComboBox", "end_dt", endDt);


3.4 Разрешить реакцию на изменение фильтра

    filter->EnableControls();

3.5 Запрограммировать в событии изменения значения параметра фильтра
    filter->setValue("filterName", "paramName", "paramValue");

    Пример:
    filter->setValue("AcctIdComboBox", "param", editBox->Text);

3.6 Запрограммировать в событии сброса фильтра
    filter->clearAllValues();

