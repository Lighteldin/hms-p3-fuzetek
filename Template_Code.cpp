#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <chrono>
#include <iomanip>
using namespace std;

#define TEST_BILL 300
#define PRESCRIPTION_BILL 100
#define GENERAL_WARD_BILL 500
#define ICU_BILL 3000
#define PRIVATE_ROOM_BILL 1500
#define SEMI_PRIVATE_ROOM_BILL 1000

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== EMERGENCY CASE CLASS ========== //
// Advanced Feature: priority_queue
class EmergencyCase {
private:
    int patientId;
    int severity;

public:
    EmergencyCase(int pid, int s) {
        patientId = pid;
        severity = s;
    }

    int getPatientId() const {
        return patientId;
    }

    int getSeverity() const {
        return severity;
    }

    // Higher severity = higher priority
    bool operator<(const EmergencyCase& other) const {
        return severity < other.severity;
    }
};


// ========== PATIENT CLASS ========== //

class Patient {
private:
    int id;
    string name;
    int age;
    string contact;
    vector<string> all_contacts;

    // Data Structures
    stack<string> medicalHistory;
    queue<string> testQueue;
    vector<string> prescriptions;

    bool isAdmitted;
    RoomType roomType;

    // Advanced Feature: Billing
    double bill;

public:
    // Constructor
    Patient(int pid, string n, int a, string c)
    {
        id = pid;
        name = n;
        age = a;
        contact = c;
        all_contacts.push_back(c);
        isAdmitted = false;
        bill = 0;
    }

    Patient(int pid, string n, int a, vector<string> contacts)
    {
        id = pid;
        name = n;
        age = a;
        all_contacts = contacts;
    }

    // ========== ORIGINAL FEATURES ========== //

    void admitPatient(RoomType type)
    {
        if (getAdmissionStatus())
        {
            std::cout << "They are already addmitted" << std::endl;
            return;
        }

        roomType = type;

        isAdmitted = true;

        switch (getRoomType())
        {
            case GENERAL_WARD: bill += GENERAL_WARD_BILL; break;
            case ICU: bill += ICU_BILL; break;
            case PRIVATE_ROOM: bill += PRIVATE_ROOM_BILL; break;
            case SEMI_PRIVATE: bill += SEMI_PRIVATE_ROOM_BILL; break;
        }
    }

    static auto get_date()
    {
        auto now = std::chrono::system_clock::now();
        return std::chrono::current_zone()->to_local(now);
    }

    void dischargePatient()
    {
        if (getAdmissionStatus())
        {
            isAdmitted = false;

            auto current_date = get_date();

            medicalHistory.push(std::format("At {:%Y-%m-%d}: Patient {:2} was discharged", current_date, name));
        }
    }

    void addMedicalRecord(string record)
    {
        medicalHistory.push(record);
    }

    void requestTest(string testName)
    {
        testQueue.push(testName);
        medicalHistory.push(std::format("At {:%Y-%m-%d}: Patient {:2} requested the {:3} test", get_date(), name, testName));
    }

    string performTest()
    {
        if (!testQueue.empty())
        {
            string t_name = testQueue.front();

            testQueue.pop();

            medicalHistory.push(std::format("At {:%Y-%m-%d}: Patient {:2} finished performing the {:3} test, added {:4} to patient bill", get_date(), name, testQueue.front(), TEST_BILL));
            addBill(TEST_BILL);

            return format("Test {:1} Performed Successfully", t_name);

        }

        return "Not Test Selected";
    }

    void displayHistory() const
    {
        auto q = medicalHistory;

        for (int i = 0; i < q.size(); ++i)
        {
            if (q.empty())
                return;

            cout << q.top() << endl;

            q.pop();
        }

        delete &q;
    }

    int getId();
    string getName();

    bool getAdmissionStatus()
    {
        return isAdmitted;
    }


    // ========== NEW FEATURES ========== //

    // Medical Tests
    void displayPendingTests() const
    {
        auto q = testQueue;

        for (int i = 0; i < q.size(); ++i)
        {
            if (q.empty())
                return;

            cout << q.front() << endl;

            q.pop();
        }

        delete &q;
    }

    // Prescriptions
    void addPrescription(string medicine)
    {
        prescriptions.push_back(medicine);

        cout << format("At {:%Y-%m-%d}: Patient {:2} took the {:3} medicine", get_date(), name, medicine);

        addBill(PRESCRIPTION_BILL);
    }

    void displayPrescriptions() const {

        for (int i = 0; i < prescriptions.size(); ++i)
        {
            if (prescriptions.empty())
                return;

            cout << prescriptions.at(i) << endl;

        }
    }

    // Billing
    void addBill(double amount) { bill += amount; }

    double getBill() const { return bill; }

    void displayBill() { cout << format("Patient {:1} is required to pay {:2}", name, bill) << endl; }

    // Additional Getters
    int getAge() { return age; }

    string getContact() { return contact; }

    void displayAllContacts()
    {
        if (all_contacts.empty()) {

            std::cout << "No contacts found." << std::endl;

            return;
        }

        for (int i = 0; i < all_contacts.size(); ++i) {

            std::cout << i + 1 << ". " << all_contacts[i] << std::endl;
        }
    }

    vector<string> getAllContact()
    {
        return all_contacts;
    }

    RoomType getRoomType()
    {
        return roomType;
    }
};

// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;

    // Queue of patients waiting for doctor
    queue<int> appointmentQueue;

public:
    // Constructor
    Doctor(int did, string n, Department d);

    // ========== ORIGINAL FEATURES ========== //

    void addAppointment(int patientId);
    int seePatient();

    int getId();
    string getName();
    string getDepartment();


    // ========== NEW FEATURES ========== //

    // Display waiting patients
    void displayAppointments();

    // Cancel appointment
    void cancelAppointment(int patientId);

    // Number of waiting patients
    int getAppointmentCount();
};


// ========== HOSPITAL CLASS ========== //
class Hospital {
private:

    // Main collections
    vector<Patient> patients;
    vector<Doctor> doctors;

    // Original emergency queue
    queue<int> emergencyQueue;

    // Advanced emergency queue
    priority_queue<EmergencyCase> priorityEmergencyQueue;

    // Counters
    int patientCounter;
    int doctorCounter;

    // ========== ROOM MANAGEMENT ========== //

    int generalRooms;
    int icuRooms;
    int privateRooms;
    int semiPrivateRooms;


public:

    // Constructor
    Hospital();


    // =====================================================
    // ORIGINAL FEATURES
    // ===================================================== //

    int registerPatient(
        string name,
        int age,
        string contact
    );

    int addDoctor(
        string name,
        Department dept
    );

    void admitPatient(
        int patientId,
        RoomType type
    );

    void addEmergency(
        int patientId
    );

    int handleEmergency();

    void bookAppointment(
        int doctorId,
        int patientId
    );

    void displayPatientInfo(
        int patientId
    );

    void displayDoctorInfo(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 1
    // Find Patient
    // ===================================================== //

    Patient* findPatient(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 2
    // Find Doctor
    // ===================================================== //

    Doctor* findDoctor(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 3
    // Search Patient By Name
    // ===================================================== //

    void searchPatientByName(
        string name
    );


    // =====================================================
    // NEW FEATURE 4
    // Discharge Patient
    // ===================================================== //

    void dischargePatient(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 5
    // Request Medical Test
    // ===================================================== //

    void requestPatientTest(
        int patientId,
        string testName
    );


    // =====================================================
    // NEW FEATURE 6
    // Perform Medical Test
    // ===================================================== //

    void performPatientTest(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 7
    // Display Pending Tests
    // ===================================================== //

    void displayPatientTests(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 8
    // Add Prescription
    // ===================================================== //

    void prescribeMedicine(
        int patientId,
        string medicine
    );


    // =====================================================
    // NEW FEATURE 9
    // Display Prescriptions
    // ===================================================== //

    void displayPrescriptions(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 10
    // Patient Bill
    // ===================================================== //

    void displayPatientBill(
        int patientId
    );


    // =====================================================
    // NEW FEATURE 11
    // Priority Emergency
    // ===================================================== //

    void addPriorityEmergency(
        int patientId,
        int severity
    );


    // =====================================================
    // NEW FEATURE 12
    // Handle Priority Emergency
    // ===================================================== //

    int handlePriorityEmergency();


    // =====================================================
    // NEW FEATURE 13
    // Room Availability
    // ===================================================== //

    bool isRoomAvailable(
        RoomType type
    );


    // =====================================================
    // NEW FEATURE 14
    // Display Room Status
    // ===================================================== //

    void displayRoomStatus();


    // =====================================================
    // NEW FEATURE 15
    // Display All Patients
    // ===================================================== //

    void displayAllPatients();


    // =====================================================
    // NEW FEATURE 16
    // Display All Doctors
    // ===================================================== //

    void displayAllDoctors();


    // =====================================================
    // NEW FEATURE 17
    // Display Doctor Appointments
    // ===================================================== //

    void displayDoctorAppointments(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 18
    // Cancel Appointment
    // ===================================================== //

    void cancelAppointment(
        int doctorId,
        int patientId
    );


    // =====================================================
    // NEW FEATURE 19
    // Doctor Sees Next Patient
    // ===================================================== //

    void doctorSeePatient(
        int doctorId
    );


    // =====================================================
    // NEW FEATURE 20
    // Hospital Statistics
    // ===================================================== //

    void displayStatistics();
};


// ========== MAIN PROGRAM ========== //
int main() {

    Hospital hospital;


    // =====================================================
    // TEST CASE 1
    // Registering patients
    // ===================================================== //

    int p1 =
        hospital.registerPatient(
            "John Doe",
            35,
            "555-1234"
        );

    int p2 =
        hospital.registerPatient(
            "Jane Smith",
            28,
            "555-5678"
        );

    int p3 =
        hospital.registerPatient(
            "Mike Johnson",
            45,
            "555-9012"
        );


    // =====================================================
    // TEST CASE 2
    // Adding doctors
    // ===================================================== //

    int d1 =
        hospital.addDoctor(
            "Dr. Smith",
            CARDIOLOGY
        );

    int d2 =
        hospital.addDoctor(
            "Dr. Brown",
            NEUROLOGY
        );

    int d3 =
        hospital.addDoctor(
            "Dr. Lee",
            PEDIATRICS
        );


    // =====================================================
    // TEST CASE 3
    // Admitting patients
    // ===================================================== //

    hospital.admitPatient(
        p1,
        PRIVATE_ROOM
    );

    hospital.admitPatient(
        p2,
        ICU
    );

    // Try admitting already admitted patient
    hospital.admitPatient(
        p1,
        SEMI_PRIVATE
    );


    // =====================================================
    // TEST CASE 4
    // Booking appointments
    // ===================================================== //

    hospital.bookAppointment(
        d1,
        p1
    );

    hospital.bookAppointment(
        d1,
        p2
    );

    hospital.bookAppointment(
        d2,
        p3
    );

    // Invalid doctor
    hospital.bookAppointment(
        999,
        p1
    );

    // Invalid patient
    hospital.bookAppointment(
        d1,
        999
    );


    // =====================================================
    // TEST CASE 5
    // Handling medical tests
    // ===================================================== //

    hospital.requestPatientTest(
        p1,
        "Blood Test"
    );

    hospital.requestPatientTest(
        p1,
        "X-Ray"
    );

    hospital.requestPatientTest(
        p1,
        "MRI"
    );

    hospital.displayPatientTests(
        p1
    );

    hospital.performPatientTest(
        p1
    );

    hospital.displayPatientTests(
        p1
    );


    // =====================================================
    // TEST CASE 6
    // Emergency cases
    // ===================================================== //

    hospital.addEmergency(p3);

    hospital.addEmergency(p1);

    int emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    emergencyPatient =
        hospital.handleEmergency();

    // No more emergencies


    // =====================================================
    // TEST CASE 7
    // Discharging patients
    // ===================================================== //

    hospital.dischargePatient(
        p1
    );


    // =====================================================
    // TEST CASE 8
    // Displaying information
    // ===================================================== //

    hospital.displayPatientInfo(
        p1
    );

    hospital.displayPatientInfo(
        p2
    );

    hospital.displayPatientInfo(
        999
    );


    hospital.displayDoctorInfo(
        d1
    );

    hospital.displayDoctorInfo(
        d2
    );

    hospital.displayDoctorInfo(
        999
    );


    // =====================================================
    // TEST CASE 9
    // Doctor seeing patients
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.doctorSeePatient(
        d1
    );

    hospital.displayDoctorAppointments(
        d1
    );


    // =====================================================
    // TEST CASE 10
    // Search Patient
    // ===================================================== //

    hospital.searchPatientByName(
        "John Doe"
    );

    hospital.searchPatientByName(
        "Unknown Patient"
    );


    // =====================================================
    // TEST CASE 11
    // Prescriptions
    // ===================================================== //

    hospital.prescribeMedicine(
        p1,
        "Paracetamol"
    );

    hospital.prescribeMedicine(
        p1,
        "Antibiotic"
    );

    hospital.displayPrescriptions(
        p1
    );


    // =====================================================
    // TEST CASE 12
    // Patient Billing
    // ===================================================== //

    hospital.displayPatientBill(
        p1
    );

    hospital.displayPatientBill(
        p2
    );


    // =====================================================
    // TEST CASE 13
    // Priority Emergency
    // ===================================================== //

    hospital.addPriorityEmergency(
        p1,
        2
    );

    hospital.addPriorityEmergency(
        p2,
        5
    );

    hospital.addPriorityEmergency(
        p3,
        3
    );

    hospital.addPriorityEmergency(
        p1,
        4
    );


    // =====================================================
    // TEST CASE 14
    // Handle Priority Emergencies
    // ===================================================== //

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();

    hospital.handlePriorityEmergency();


    // =====================================================
    // TEST CASE 15
    // Room Management
    // ===================================================== //

    hospital.displayRoomStatus();


    // =====================================================
    // TEST CASE 16
    // Display All Patients
    // ===================================================== //

    hospital.displayAllPatients();


    // =====================================================
    // TEST CASE 17
    // Display All Doctors
    // ===================================================== //

    hospital.displayAllDoctors();


    // =====================================================
    // TEST CASE 18
    // Cancel Appointment
    // ===================================================== //

    hospital.cancelAppointment(
        d1,
        p2
    );


    // =====================================================
    // TEST CASE 19
    // More Doctor Appointments
    // ===================================================== //

    hospital.displayDoctorAppointments(
        d1
    );

    hospital.displayDoctorAppointments(
        d2
    );


    // =====================================================
    // TEST CASE 20
    // Hospital Statistics
    // ===================================================== //

    hospital.displayStatistics();


    // =====================================================
    // TEST CASE 21
    // Edge Cases
    // ===================================================== //

    Hospital emptyHospital;

    emptyHospital.displayPatientInfo(
        1
    );

    emptyHospital.displayDoctorInfo(
        1
    );

    emptyHospital.handleEmergency();

    emptyHospital.handlePriorityEmergency();

    emptyHospital.searchPatientByName(
        "John Doe"
    );

    emptyHospital.displayAllPatients();

    emptyHospital.displayAllDoctors();

    emptyHospital.displayStatistics();


    return 0;
}