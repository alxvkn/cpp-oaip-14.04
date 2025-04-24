#include <memory>

#include <wx/wx.h>
#include <wx/numdlg.h>

#include "RatesProvider.hpp"

// Создайте оконное приложение для конвертации валют. Напишите программу,
// которая переводит российские рубли в белорусские рубли. Программа должна
// иметь предустановленный валютный курс, который пользователь сможет при
// необходимости изменить.

class ConverterFrame : public wxFrame {
public:
    ConverterFrame(const wxString& title, std::unique_ptr<RatesProvider> _rates_provider) 
    : wxFrame(NULL, wxID_ANY, title), value_left(0.0), value_right(0.0), rates_provider(std::move(_rates_provider)) {

        wxPanel* panel = new wxPanel(this, wxID_ANY);
        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
        wxGridSizer* grid = new wxGridSizer(4, 2, 0, 0);

        input_left = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
        input_left->SetValidator(wxTextValidator(wxFILTER_NUMERIC));

        input_right = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
        input_right->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
        // input_right->SetEditable(false);

        wxArrayString choices;

        for (auto c : rates_provider->rates()) {
            choices.Add(c.first);
        }

        choice_left = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(100, -1),
                                    choices);
        choice_left->SetStringSelection("RUB");

        choice_right = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(100, -1),
                                    choices);
        choice_right->SetStringSelection("BYN");

        custom_rate_checkbox = new wxCheckBox(panel, wxID_ANY, wxT("Свой курс"));
        custom_rate_input = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
        custom_rate_input->SetValidator(wxTextValidator(wxFILTER_NUMERIC));
        custom_rate_input->Hide();

        grid->Add(input_left, 0, wxALL, 10);
        grid->Add(input_right, 0, wxALL, 10);
        grid->Add(choice_left, 0, wxBOTTOM | wxLEFT | wxRIGHT, 10);
        grid->Add(choice_right, 0, wxBOTTOM | wxLEFT | wxRIGHT, 10);
        grid->Add(0, 0);
        grid->Add(custom_rate_checkbox, 1, wxTOP | wxLEFT | wxRIGHT, 10);
        grid->Add(0, 0);
        grid->Add(custom_rate_input, 0, wxBOTTOM | wxLEFT | wxRIGHT | wxALIGN_RIGHT, 10);
        panel->SetSizer(grid);

        grid->SetSizeHints(this);

        input_left->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) { OnLeftInputChange(event); });
        input_right->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) { OnRightInputChange(event); });
        choice_left->Bind(wxEVT_CHOICE, [this](wxCommandEvent& event) { OnLeftChoice(event); });
        choice_right->Bind(wxEVT_CHOICE, [this](wxCommandEvent& event) { OnRightChoice(event); });
        custom_rate_checkbox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event) { OnCustomRateCheckboxChange(event); });
        custom_rate_input->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) { OnCustomRateInputChange(event); });

        rate_left = rates_provider->rates().at(choice_left->GetStringSelection().ToStdString());
        rate_right = rates_provider->rates().at(choice_right->GetStringSelection().ToStdString());

        update();
    }

private:
    wxTextCtrl* input_left;
    wxTextCtrl* input_right;

    wxChoice* choice_left;
    wxChoice* choice_right;

    wxCheckBox* custom_rate_checkbox;
    bool custom_rate_state = false;

    wxTextCtrl* custom_rate_input;
    double custom_rate = 0;

    double value_left, value_right,
        rate_left, rate_right;

    const std::unique_ptr<RatesProvider> rates_provider;

    void OnCustomRateCheckboxChange(wxCommandEvent& event) {
        custom_rate_state = custom_rate_checkbox->GetValue();

        choice_left->Enable(!custom_rate_state);
        choice_right->Enable(!custom_rate_state);

        if (custom_rate_state)
            custom_rate_input->Show();
        else
            custom_rate_input->Hide();

        value_right = value_left * get_rate();
        update();
    }

    void OnCustomRateInputChange(wxCommandEvent& event) {
        custom_rate_input->GetValue().ToDouble(&custom_rate);

        value_right = value_left * get_rate();

        update();
    }

    void OnLeftChoice(wxCommandEvent& event) {
        rate_left = rates_provider->rates().at(choice_left->GetStringSelection().ToStdString());

        value_right = value_left * get_rate();
        update();
    }

    void OnRightChoice(wxCommandEvent& event) {
        rate_right = rates_provider->rates().at(choice_right->GetStringSelection().ToStdString());

        value_right = value_left * get_rate();
        update();
    }

    void OnLeftInputChange(wxCommandEvent& event) {
        input_left->GetValue().ToDouble(&value_left);

        value_right = value_left * get_rate();
        update();
    }

    void OnRightInputChange(wxCommandEvent& event) {
        input_right->GetValue().ToDouble(&value_right);

        value_left = value_right / get_rate();
        update();
    }

    double get_rate() {
        if (custom_rate_state) {
            return custom_rate;
        }
        return rate_right / rate_left;
    }

    void update() {
        input_left->ChangeValue(wxString::FromDouble(value_left));
        input_right->ChangeValue(wxString::FromDouble(value_right));

        input_right->Refresh();
        input_left->Refresh();
    }
};

class ConverterApp : public wxApp {
public:
    virtual bool OnInit() {
        ConverterFrame* frame = new ConverterFrame(wxT("Конвертер валют"), std::make_unique<RatesProvider>("RUB"));
        frame->Show(true);

        return true;
    }
};

wxIMPLEMENT_APP(ConverterApp);
