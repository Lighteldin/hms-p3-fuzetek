#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;


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
    Patient(int pid, string n, int a, string c){
        id = pid;
        name = n;
        age = a;
        contact = c;
        isAdmitted = false;
        bill = 0;
        // roomType intentionally left uninitialized — only meaningful once admitted, per SRS §3.2
    }

    // ========== ORIGINAL FEATURES ========== //

    void admitPatient(RoomType type) {
    if (isAdmitted) {
        cout << "Patient is already admitted." << endl;
        return;
    }

    isAdmitted = true;
    roomType = type;
    addMedicalRecord("Patient admitted to hospital");

    switch (type) {
        case GENERAL_WARD:
            bill += 500;
            break;
        case ICU:
            bill += 3000;
            break;
        case PRIVATE_ROOM:
            bill += 1500;
            break;
        case SEMI_PRIVATE:
            bill += 1000;
            break;
    }
}
    void dischargePatient() {
    if (!isAdmitted) {
        cout << "Patient is not currently admitted." << endl;
        return;
    }

    isAdmitted = false;
    addMedicalRecord("Patient discharged from hospital");
}

    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }

    void requestTest(string testName);
    string performTest();

    void displayHistory() {
        cout << "Medical History for " << name << " (ID: " << id << "):" << endl;

        stack<string> temp = medicalHistory;

        while (!temp.empty()) {
            cout << "- " << temp.top() << endl;
            temp.pop();
        }
    }

    int getId(){
        return id;
    }
    string getName(){
        return name;
    }

    bool getAdmissionStatus() {
        return isAdmitted;
    }


    // ========== NEW FEATURES ========== //

    // Medical Tests
    void displayPendingTests();


    // Prescriptions
    void addPrescription(string medicine);
    void displayPrescriptions();

    // Billing 
    void addBill(double amount);
    double getBill();
    void displayBill();

    // Additional Getters
    int getAge(){
        return age;
    }
    string getContact(){
        return contact;
    }
    RoomType getRoomType(){
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
    Doctor(int did, string n, Department d) {
        id = did;
        name = n;
        department = d;
    }

    // ========== ORIGINAL FEATURES ========== //

    void addAppointment(int patientId) {
        appointmentQueue.push(patientId);
    }
    int seePatient() {
    if (appointmentQueue.empty()) {
        return -1;
    }

    int patientId = appointmentQueue.front();
    appointmentQueue.pop();

    return patientId;
}

    int getId() {
        return id;
    }
    string getName(){
        return name;
    }
    string getDepartment(){
        switch (department) {
            case CARDIOLOGY:
                return "Cardiology";

            case NEUROLOGY:
                return "Neurology";

            case ORTHOPEDICS:
                return "Orthopedics";

            case PEDIATRICS:
                return "Pediatrics";

            case EMERGENCY:
                return "Emergency";

            case GENERAL:
                return "General";

            default:
                return "General";
        }
    }


    // ========== NEW FEATURES ========== //

    // Display waiting patients
    void displayAppointments(){
        if (appointmentQueue.empty()) {
            cout << "No appointments." << endl;
            return;
        }

        cout << "Appointment Queue:" << endl;

        queue<int> temp = appointmentQueue;

        while (!temp.empty()) {
            cout << "- Patient ID: " << temp.front() << endl;
            temp.pop();
        }
    }

    // Cancel appointment
    void cancelAppointment(int patientId) {
        if (appointmentQueue.empty()) {
            cout << "No appointments available." << endl;
            return;
        }

        queue<int> temp;
        bool found = false;

        while (!appointmentQueue.empty()) {
            int currentPatient = appointmentQueue.front();
            appointmentQueue.pop();

            if (currentPatient == patientId && !found) {
                found = true;
            }
            else {
                temp.push(currentPatient);
            }
        }

        appointmentQueue = temp;

        if (found) {
            cout << "Appointment cancelled successfully." << endl;
        }
        else {
            cout << "Appointment not found." << endl;
        }
    }

    // Number of waiting patients
    int getAppointmentCount() {
        return appointmentQueue.size();
    }
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
    Hospital()
    {
        patientCounter = 1;
        doctorCounter = 1;
        generalRooms = 20;
        icuRooms = 5;
        privateRooms = 10;
        semiPrivateRooms = 10;
    }


    // =====================================================
    // ORIGINAL FEATURES
    // ===================================================== //

    int registerPatient(
        string name,
        int age,
        string contact
    )
    {
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        int assignedId = patientCounter;
        patientCounter++;
        return assignedId;
    }


    int addDoctor(
        string name,
        Department dept
    )
    {
        Doctor newDoctor(doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        int assignedId = doctorCounter;
        doctorCounter++;
        return assignedId;
    }


    void admitPatient(
        int patientId,
        RoomType type
    )
    {
        Patient* p = findPatient(patientId);
        if (p == nullptr) {
            cout << "Patient with ID " << patientId << " not found." << endl;
            return;
        }
        if (!isRoomAvailable(type)) {
            cout << "No room available for this room type." << endl;
            return;
        }
        p->admitPatient(type);
    }


    void addEmergency(
        int patientId
    )
     {
        emergencyQueue.push(patientId);
     }

    int handleEmergency()
    {
        if (emergencyQueue.empty()) {
            cout << "No emergencies in queue." << endl;
            return -1;
        }
        int patientId = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handled emergency for patient: " << patientId << endl;
        return patientId;
    }



    void bookAppointment(
        int doctorId,
        int patientId
    )
    {
        Doctor* d = findDoctor(doctorId);
        Patient* p = findPatient(patientId);

        if (d == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
        if (p == nullptr) {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
        if (d == nullptr || p == nullptr) {
            return;
        }

        d->addAppointment(patientId);
        cout << "Appointment booked for patient " << patientId << " with doctor " << doctorId << endl;
    }

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
    )
    {
        Patient* p = findPatient(patientId);
        if (p == nullptr) {
            cout << "Patient with ID " << patientId << " not found." << endl;
            return;
        }
        if (severity < 1 || severity > 5) {
            cout << "Invalid severity level. Must be between 1 and 5." << endl;
            return;
        }
        priorityEmergencyQueue.push(EmergencyCase(patientId, severity));
        cout << "Emergency added with severity " << severity << endl;
    }


    // =====================================================
    // NEW FEATURE 12
    // Handle Priority Emergency
    // ===================================================== //

    int handlePriorityEmergency()
    {
        if (priorityEmergencyQueue.empty()) {
            cout << "No priority emergencies." << endl;
            return -1;
        }
        EmergencyCase topCase = priorityEmergencyQueue.top();
        priorityEmergencyQueue.pop();
        int patientId = topCase.getPatientId();
        int severity = topCase.getSeverity();
        cout << "Handling patient " << patientId << " with severity " << severity << endl;
        return patientId;
    }


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
    )
    {
        Doctor* d = findDoctor(doctorId);
        if (d == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
            return;
        }
        cout << "Appointments for " << d->getName() << ":" << endl;
        d->displayAppointments();
    }


    // =====================================================
    // NEW FEATURE 18
    // Cancel Appointment
    // ===================================================== //

    void cancelAppointment(
        int doctorId,
        int patientId
    )
     {
        Doctor* d = findDoctor(doctorId);
        if (d == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
            return;
        }
        d->cancelAppointment(patientId);
     }


    // =====================================================
    // NEW FEATURE 19
    // Doctor Sees Next Patient
    // ===================================================== //

    void doctorSeePatient(
        int doctorId
    )
     {
        Doctor* d = findDoctor(doctorId);
        if (d == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
            return;
        }
        int patientId = d->seePatient();
        if (patientId == -1) {
            cout << "No patients waiting." << endl;
        } else {
            cout << d->getName() << " is now seeing patient " << patientId << endl;
        }
     }


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
