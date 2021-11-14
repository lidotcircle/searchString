#ifndef _SUPPORTED_TRAINER_KERNEL_H_
#define _SUPPORTED_TRAINER_KERNEL_H_


#define ngram_map(trainer_l, trainer, kernel_l, kernel) \
    NENTRY(1, 1, trainer_l, trainer, kernel_l, kernel) \
    NENTRY(2, 2, trainer_l, trainer, kernel_l, kernel) \
    NENTRY(3, 3, trainer_l, trainer, kernel_l, kernel)

#define trainer_map(kernel_l, kernel) \
    ngram_map("c",     dlib::svm_c_trainer, kernel_l, kernel) \
    ngram_map("c_ekm", dlib::svm_c_ekm_trainer, kernel_l, kernel)

#define kernel_map() \
    trainer_map("linear",  dlib::linear_kernel) \
    trainer_map("poly",    dlib::polynomial_kernel) \
    trainer_map("rbf",     dlib::radial_basis_kernel) \
    trainer_map("sigmoid", dlib::sigmoid_kernel)


#endif // _SUPPORTED_TRAINER_KERNEL_H_
