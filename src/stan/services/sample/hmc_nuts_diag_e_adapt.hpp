#ifndef STAN_SERVICES_SAMPLE_HMC_NUTS_DIAG_E_ADAPT_HPP
#define STAN_SERVICES_SAMPLE_HMC_NUTS_DIAG_E_ADAPT_HPP

#include <stan/math/prim/mat/fun/Eigen.hpp>
#include <stan/interface_callbacks/interrupt/base_interrupt.hpp>
#include <stan/interface_callbacks/writer/base_writer.hpp>
#include <stan/mcmc/fixed_param_sampler.hpp>
#include <stan/services/error_codes.hpp>
#include <stan/services/check_timing.hpp>
#include <stan/services/sample/run_adaptive_sampler.hpp>
#include <stan/mcmc/hmc/nuts/adapt_diag_e_nuts.hpp>
#include <ctime>

namespace stan {
  namespace services {
    namespace sample {

      /**
       * Runs HMC with NUTS with diagonal Euclidean
       * metric without adapatation.
       *
       * @tparam Model Model class
       * @tparam rng_t Random number generator class
       * @param model Instance of model
       * @param base_rng Instance of random number generator
       * @param cont_params Initial value
       * @param num_samples Number of samples
       * @param num_thin Number to thin the samples
       * @param refresh Controls the output
       * @param interrupt Callback for interrupts
       * @param sample_writer Writer for draws
       * @param diagnostic_writer Writer for diagnostic information
       * @param message_writer Writer for messages
       * @return error code; 0 if no error
       */
      template <class Model, class rng_t>
      int hmc_nuts_diag_e_adapt(Model& model,
                                rng_t& base_rng,
                                Eigen::VectorXd& cont_params,
                                int num_warmup,
                                int num_samples,
                                int num_thin,
                                bool save_warmup,
                                int refresh,
                                double stepsize,
                                double stepsize_jitter,
                                int max_depth,
                                double delta,
                                double gamma,
                                double kappa,
                                double t0,
                                unsigned int init_buffer,
                                unsigned int term_buffer,
                                unsigned int window,
                                interface_callbacks::interrupt::base_interrupt& interrupt,
                                interface_callbacks::writer::base_writer& sample_writer,
                                interface_callbacks::writer::base_writer& diagnostic_writer,
                                interface_callbacks::writer::base_writer& message_writer) {
        stan::services::check_timing(model, cont_params, message_writer);

        stan::mcmc::adapt_diag_e_nuts<Model, rng_t> sampler(model, base_rng);
        sampler.set_nominal_stepsize(stepsize);
        sampler.set_stepsize_jitter(stepsize_jitter);
        sampler.set_max_depth(max_depth);

        sampler.get_stepsize_adaptation().set_mu(log(10 * stepsize));
        sampler.get_stepsize_adaptation().set_delta(delta);
        sampler.get_stepsize_adaptation().set_gamma(gamma);
        sampler.get_stepsize_adaptation().set_kappa(kappa);
        sampler.get_stepsize_adaptation().set_t0(t0);
         
        sampler.set_window_params(num_warmup, init_buffer, term_buffer,
                                  window, message_writer);

        run_adaptive_sampler(sampler, model,
                             cont_params,
                             num_warmup, num_samples, num_thin,
                             refresh, save_warmup, base_rng,
                             interrupt, sample_writer, diagnostic_writer,
                             message_writer);
        
        return stan::services::error_codes::OK;
      }
      
    }
  }
}
#endif